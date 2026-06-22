#include "Command.h"

#include "Robot.h"

#include <algorithm>
#include <charconv>
#include <cctype>
#include <cstdint>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

namespace Simulator
{
namespace
{

[[nodiscard]] std::string uppercase(std::string value)
{
    std::ranges::transform(value, value.begin(), [](unsigned char character)
                           { return static_cast<char>(std::toupper(character)); });
    return value;
}

[[nodiscard]] std::vector<std::string> tokenize(std::string_view input)
{
    std::string normalized{input};
    std::ranges::replace(normalized, ',', ' ');

    std::istringstream stream{normalized};
    std::vector<std::string> tokens;
    for (std::string token; stream >> token;)
    {
        tokens.push_back(std::move(token));
    }
    return tokens;
}

template <typename Value> [[nodiscard]] std::optional<Value> parseInteger(std::string_view text)
{
    Value value{};
    const auto *first = text.data();
    const auto *last =
        first + text.size(); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    const auto [position, error] = std::from_chars(first, last, value);
    if (error != std::errc{} || position != last)
    {
        return std::nullopt;
    }
    return value;
}

[[nodiscard]] std::optional<RobotFactory::Direction> parseDirection(std::string value)
{
    value = uppercase(std::move(value));
    if (value == "NORTH")
    {
        return RobotFactory::Direction::North;
    }
    if (value == "EAST")
    {
        return RobotFactory::Direction::East;
    }
    if (value == "SOUTH")
    {
        return RobotFactory::Direction::South;
    }
    if (value == "WEST")
    {
        return RobotFactory::Direction::West;
    }
    return std::nullopt;
}

[[nodiscard]] std::optional<RobotFactory::Rotation> parseRotation(std::string value)
{
    value = uppercase(std::move(value));
    if (value == "LEFT")
    {
        return RobotFactory::Rotation::Left;
    }
    if (value == "RIGHT")
    {
        return RobotFactory::Rotation::Right;
    }
    return std::nullopt;
}

[[nodiscard]] std::optional<RobotTarget> parseTarget(std::string value)
{
    if (value.starts_with('@'))
    {
        const auto id = parseInteger<RobotFactory::RobotId>(std::string_view{value}.substr(1));
        if (!id || *id == 0)
        {
            return std::nullopt;
        }
        return RobotTarget{*id};
    }

    value = uppercase(std::move(value));
    if (value.empty() || value == "ALL")
    {
        return std::nullopt;
    }
    return RobotTarget{std::move(value)};
}

[[nodiscard]] ParseResult failure(std::string message)
{
    return {.command = std::nullopt, .error = std::move(message)};
}

[[nodiscard]] ParseResult success(Command command)
{
    return {.command = std::move(command), .error = {}};
}

} // namespace

ParseResult::operator bool() const noexcept
{
    return command.has_value();
}

ParseResult CommandParser::parse(std::string_view input)
{
    auto tokens = tokenize(input);
    if (tokens.empty())
    {
        return failure("Command is empty.");
    }

    const std::string verb = uppercase(tokens.front());
    if (verb == "PLACE")
    {
        if (tokens.size() != 5)
        {
            return failure("Usage: PLACE <name> <x>,<y> <direction>.");
        }
        if (tokens.at(1).starts_with('@'))
        {
            return failure("Robot names cannot begin with '@'.");
        }
        const auto x = parseInteger<RobotFactory::Coordinate>(tokens.at(2));
        const auto y = parseInteger<RobotFactory::Coordinate>(tokens.at(3));
        const auto direction = parseDirection(tokens.at(4));
        if (!x || !y || *x < 0 || *y < 0 || !direction)
        {
            return failure("PLACE requires non-negative coordinates and a valid direction.");
        }
        return success(PlaceCommand{.name = uppercase(tokens.at(1)),
                                    .location = {.x = *x, .y = *y, .direction = *direction}});
    }

    if (verb == "MOVE")
    {
        if (tokens.size() > 3)
        {
            return failure("Usage: MOVE [ALL|name|@id] [blocks].");
        }
        MoveCommand command;
        if (tokens.size() >= 2 && uppercase(tokens.at(1)) != "ALL")
        {
            command.target = parseTarget(tokens.at(1));
            if (!command.target)
            {
                return failure("MOVE target is invalid.");
            }
        }
        if (tokens.size() == 3)
        {
            const auto blocks = parseInteger<std::uint32_t>(tokens.at(2));
            if (!blocks || *blocks == 0)
            {
                return failure("MOVE blocks must be a positive integer.");
            }
            command.blocks = *blocks;
        }
        return success(command);
    }

    if (verb == "ROTATE")
    {
        if (tokens.size() != 2 && tokens.size() != 3)
        {
            return failure("Usage: ROTATE [ALL|name|@id] <LEFT|RIGHT>.");
        }
        RotateCommand command;
        const auto rotation_index = tokens.size() - 1;
        const auto rotation = parseRotation(tokens.at(rotation_index));
        if (!rotation)
        {
            return failure("ROTATE requires LEFT or RIGHT.");
        }
        command.rotation = *rotation;
        if (tokens.size() == 3 && uppercase(tokens.at(1)) != "ALL")
        {
            command.target = parseTarget(tokens.at(1));
            if (!command.target)
            {
                return failure("ROTATE target is invalid.");
            }
        }
        return success(command);
    }

    if (verb == "LEFT" || verb == "RIGHT")
    {
        if (tokens.size() > 2)
        {
            return failure("Usage: LEFT|RIGHT [ALL|name|@id].");
        }
        RotateCommand command{.target = std::nullopt,
                              .rotation = verb == "LEFT" ? RobotFactory::Rotation::Left
                                                         : RobotFactory::Rotation::Right};
        if (tokens.size() == 2 && uppercase(tokens.at(1)) != "ALL")
        {
            command.target = parseTarget(tokens.at(1));
            if (!command.target)
            {
                return failure("Rotation target is invalid.");
            }
        }
        return success(command);
    }

    if (verb == "REMOVE")
    {
        if (tokens.size() > 2)
        {
            return failure("Usage: REMOVE [ALL|name|@id].");
        }
        RemoveCommand command;
        if (tokens.size() == 2 && uppercase(tokens.at(1)) != "ALL")
        {
            command.target = parseTarget(tokens.at(1));
            if (!command.target)
            {
                return failure("REMOVE target is invalid.");
            }
        }
        return success(command);
    }

    if (verb == "RESIZE")
    {
        if (tokens.size() != 3)
        {
            return failure("Usage: RESIZE <width> <height>.");
        }
        const auto width = parseInteger<RobotFactory::Coordinate>(tokens.at(1));
        const auto height = parseInteger<RobotFactory::Coordinate>(tokens.at(2));
        if (!width || !height || *width <= 0 || *height <= 0)
        {
            return failure("RESIZE dimensions must be positive integers.");
        }
        return success(ResizeCommand{.size = {.width = *width, .height = *height}});
    }

    if ((verb == "REPORT" || verb == "MENU" || verb == "QUIT" || verb == "EXIT") &&
        tokens.size() != 1)
    {
        return failure(verb + " does not accept arguments.");
    }
    if (verb == "REPORT")
    {
        return success(ReportCommand{});
    }
    if (verb == "MENU")
    {
        return success(MenuCommand{});
    }
    if (verb == "QUIT" || verb == "EXIT")
    {
        return success(QuitCommand{});
    }
    return failure("Unknown command: " + verb + '.');
}

} // namespace Simulator
