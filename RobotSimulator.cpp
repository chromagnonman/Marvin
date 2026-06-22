#include "RobotSimulator.h"

#include "Command.h"
#include "Menu.h"
#include "Robot.h"
#include "RobotAssembly.h"
#include "RobotGrid.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>

namespace Simulator
{
namespace
{

[[nodiscard]] std::string canonicalName(std::string_view name)
{
    std::string canonical{name};
    std::ranges::transform(canonical, canonical.begin(), [](unsigned char character)
                           { return static_cast<char>(std::toupper(character)); });
    return canonical;
}

} // namespace

class RobotSimulator::Impl
{
  public:
    explicit Impl(GridSize size) : grid{size} {}

  private:
    friend class RobotSimulator;

    RobotGrid grid;
    std::unordered_map<std::string, std::unique_ptr<RobotFactory::Robot>> robots_by_name;
    std::unordered_map<RobotFactory::RobotId, RobotFactory::Robot *> robots_by_id;

    [[nodiscard]] RobotFactory::Robot *find(std::string_view name)
    {
        const auto robot = robots_by_name.find(canonicalName(name));
        return robot == robots_by_name.end() ? nullptr : robot->second.get();
    }

    [[nodiscard]] const RobotFactory::Robot *find(std::string_view name) const
    {
        const auto robot = robots_by_name.find(canonicalName(name));
        return robot == robots_by_name.end() ? nullptr : robot->second.get();
    }

    [[nodiscard]] RobotFactory::Robot *find(RobotFactory::RobotId id)
    {
        const auto robot = robots_by_id.find(id);
        return robot == robots_by_id.end() ? nullptr : robot->second;
    }

    [[nodiscard]] const RobotFactory::Robot *find(RobotFactory::RobotId id) const
    {
        const auto robot = robots_by_id.find(id);
        return robot == robots_by_id.end() ? nullptr : robot->second;
    }

    [[nodiscard]] bool move(RobotFactory::Robot &robot, std::uint32_t blocks)
    {
        const auto previous = robot.location();
        robot.move(blocks);
        if (grid.isOffGrid(robot.location()) || grid.isOccupied(robot.location()))
        {
            robot.setLocation(previous);
            return false;
        }
        grid.updateLocation(previous, robot);
        return true;
    }

    [[nodiscard]] bool erase(RobotFactory::Robot &robot)
    {
        const auto name = canonicalName(robot.model());
        grid.remove(robot);
        robots_by_id.erase(robot.id());
        return robots_by_name.erase(name) == 1;
    }
};

RobotSimulator::RobotSimulator() : RobotSimulator{default_grid_size} {}

RobotSimulator::RobotSimulator(GridSize size) : m_impl{std::make_unique<Impl>(size)} {}

RobotSimulator::~RobotSimulator() = default;
RobotSimulator::RobotSimulator(RobotSimulator &&) noexcept = default;
RobotSimulator &RobotSimulator::operator=(RobotSimulator &&) noexcept = default;

void RobotSimulator::start()
{
    run(std::cin, std::cout, std::cerr);
}

void RobotSimulator::run(std::istream &input, std::ostream &output, std::ostream &errors)
{
    Menu::showUsage(output);
    for (std::string line; std::getline(input, line);)
    {
        if (!executeLine(line, output, errors))
        {
            break;
        }
        output << "> ";
    }
}

bool RobotSimulator::executeLine(std::string_view line, std::ostream &output, std::ostream &errors)
{
    const auto parsed = CommandParser::parse(line);
    if (!parsed)
    {
        errors << "Error: " << parsed.error << '\n';
        return true;
    }

    return std::visit(
        [this, &output, &errors](const auto &command) -> bool
        {
            using Type = std::decay_t<decltype(command)>;
            if constexpr (std::is_same_v<Type, PlaceCommand>)
            {
                if (!place(RobotFactory::GroundRobotType::Bipedal, command.location, command.name))
                {
                    errors << "Unable to place robot; name or location is already in use.\n";
                }
            }
            else if constexpr (std::is_same_v<Type, MoveCommand>)
            {
                const auto moved = command.target
                                       ? std::visit([this, &command](const auto &target)
                                                    { return this->move(target, command.blocks); },
                                                    command.target->value)
                                       : moveAll(command.blocks) > 0;
                if (!moved)
                {
                    errors << "No robot could be moved.\n";
                }
            }
            else if constexpr (std::is_same_v<Type, RotateCommand>)
            {
                const auto rotated =
                    command.target ? std::visit([this, &command](const auto &target)
                                                { return this->rotate(target, command.rotation); },
                                                command.target->value)
                                   : rotateAll(command.rotation) > 0;
                if (!rotated)
                {
                    errors << "No matching robot was found.\n";
                }
            }
            else if constexpr (std::is_same_v<Type, RemoveCommand>)
            {
                const auto removed = command.target ? std::visit([this](const auto &target)
                                                                 { return this->remove(target); },
                                                                 command.target->value)
                                                    : removeAll() > 0;
                if (!removed)
                {
                    errors << "No matching robot was found.\n";
                }
            }
            else if constexpr (std::is_same_v<Type, ResizeCommand>)
            {
                if (!resize(command.size))
                {
                    errors << "Grid dimensions cannot shrink.\n";
                }
            }
            else if constexpr (std::is_same_v<Type, ReportCommand>)
            {
                report(output);
            }
            else if constexpr (std::is_same_v<Type, MenuCommand>)
            {
                Menu::showUsage(output);
            }
            else if constexpr (std::is_same_v<Type, QuitCommand>)
            {
                return false;
            }
            if constexpr (!std::is_same_v<Type, QuitCommand>)
            {
                return true;
            }
        },
        *parsed.command);
}

bool RobotSimulator::place(RobotFactory::GroundRobotType type, RobotFactory::RobotLocation location,
                           std::string_view name)
{
    const std::string key = canonicalName(name);
    if (key.empty() || m_impl->robots_by_name.contains(key))
    {
        return false;
    }

    auto robot = RobotFactory::RobotAssembly::create(type, location, key);
    if (!robot || !m_impl->grid.addRobot(*robot))
    {
        return false;
    }
    auto *raw_robot = robot.get();
    m_impl->robots_by_id.emplace(robot->id(), raw_robot);
    m_impl->robots_by_name.emplace(key, std::move(robot));
    return true;
}

bool RobotSimulator::move(std::string_view name, std::uint32_t blocks)
{
    auto *robot = m_impl->find(name);
    return robot != nullptr && m_impl->move(*robot, blocks);
}

bool RobotSimulator::move(RobotFactory::RobotId id, std::uint32_t blocks)
{
    auto *robot = m_impl->find(id);
    return robot != nullptr && m_impl->move(*robot, blocks);
}

std::size_t RobotSimulator::moveAll(std::uint32_t blocks)
{
    std::size_t moved{0};
    for (auto &[name, robot] : m_impl->robots_by_name)
    {
        static_cast<void>(name);
        moved += m_impl->move(*robot, blocks) ? 1U : 0U;
    }
    return moved;
}

bool RobotSimulator::rotate(std::string_view name, RobotFactory::Rotation rotation)
{
    auto *robot = m_impl->find(name);
    if (robot == nullptr)
    {
        return false;
    }
    robot->rotate(rotation);
    return true;
}

bool RobotSimulator::rotate(RobotFactory::RobotId id, RobotFactory::Rotation rotation)
{
    auto *robot = m_impl->find(id);
    if (robot == nullptr)
    {
        return false;
    }
    robot->rotate(rotation);
    return true;
}

std::size_t RobotSimulator::rotateAll(RobotFactory::Rotation rotation)
{
    for (auto &[name, robot] : m_impl->robots_by_name)
    {
        static_cast<void>(name);
        robot->rotate(rotation);
    }
    return m_impl->robots_by_name.size();
}

bool RobotSimulator::remove(std::string_view name)
{
    auto *robot = m_impl->find(name);
    return robot != nullptr && m_impl->erase(*robot);
}

bool RobotSimulator::remove(RobotFactory::RobotId id)
{
    auto *robot = m_impl->find(id);
    return robot != nullptr && m_impl->erase(*robot);
}

std::size_t RobotSimulator::removeAll()
{
    const auto count = m_impl->robots_by_name.size();
    m_impl->robots_by_id.clear();
    m_impl->robots_by_name.clear();
    m_impl->grid = RobotGrid{m_impl->grid.size()};
    return count;
}

bool RobotSimulator::resize(GridSize size)
{
    return m_impl->grid.resize(size);
}

void RobotSimulator::report(std::ostream &output) const
{
    const auto size = m_impl->grid.size();
    output << "Grid: " << size.width << 'x' << size.height
           << "\nRobots: " << m_impl->robots_by_name.size() << '\n';
    for (const auto &[name, robot] : m_impl->robots_by_name)
    {
        static_cast<void>(name);
        Menu::showDetails(*robot, output);
    }
}

const RobotFactory::Robot *RobotSimulator::findRobot(std::string_view name) const
{
    return m_impl->find(name);
}

const RobotFactory::Robot *RobotSimulator::findRobot(RobotFactory::RobotId id) const
{
    return m_impl->find(id);
}

GridSize RobotSimulator::gridSize() const noexcept
{
    return m_impl->grid.size();
}

std::size_t RobotSimulator::robotCount() const noexcept
{
    return m_impl->robots_by_name.size();
}

} // namespace Simulator
