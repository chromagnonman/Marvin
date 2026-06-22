#ifndef COMMAND_H
#define COMMAND_H

#include "Robot.h"
#include "RobotGrid.h"

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace Simulator
{

struct RobotTarget
{
    std::variant<std::string, RobotFactory::RobotId> value;
};

struct PlaceCommand
{
    std::string name;
    RobotFactory::RobotLocation location;
};

struct MoveCommand
{
    std::optional<RobotTarget> target;
    std::uint32_t blocks{1};
};

struct RotateCommand
{
    std::optional<RobotTarget> target;
    RobotFactory::Rotation rotation;
};

struct RemoveCommand
{
    std::optional<RobotTarget> target;
};

struct ResizeCommand
{
    GridSize size;
};

struct ReportCommand
{
};

struct MenuCommand
{
};

struct QuitCommand
{
};

using Command = std::variant<PlaceCommand, MoveCommand, RotateCommand, RemoveCommand, ResizeCommand,
                             ReportCommand, MenuCommand, QuitCommand>;

struct ParseResult
{
    std::optional<Command> command;
    std::string error;

    [[nodiscard]] explicit operator bool() const noexcept;
};

class CommandParser
{
  public:
    [[nodiscard]] static ParseResult parse(std::string_view input);
};

} // namespace Simulator

#endif
