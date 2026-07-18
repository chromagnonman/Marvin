#include "marvin/simulator/RobotSimulator.h"

#include "marvin/command/Command.h"
#include "marvin/robot/Robot.h"
#include "marvin/robot/RobotAssembly.h"
#include "marvin/simulator/Menu.h"
#include "marvin/simulator/RobotGrid.h"

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

RobotSimulator::RobotSimulator() : RobotSimulator{default_grid_size} {}

RobotSimulator::RobotSimulator(GridSize size) : m_grid{size} {}

RobotFactory::Robot *RobotSimulator::find(std::string_view name)
{
    const auto robot = m_robots_by_name.find(canonicalName(name));
    return robot == m_robots_by_name.end() ? nullptr : robot->second.get();
}

const RobotFactory::Robot *RobotSimulator::find(std::string_view name) const
{
    const auto robot = m_robots_by_name.find(canonicalName(name));
    return robot == m_robots_by_name.end() ? nullptr : robot->second.get();
}

RobotFactory::Robot *RobotSimulator::find(RobotFactory::RobotId id)
{
    const auto robot = m_robots_by_id.find(id);
    return robot == m_robots_by_id.end() ? nullptr : robot->second;
}

const RobotFactory::Robot *RobotSimulator::find(RobotFactory::RobotId id) const
{
    const auto robot = m_robots_by_id.find(id);
    return robot == m_robots_by_id.end() ? nullptr : robot->second;
}

bool RobotSimulator::move(RobotFactory::Robot &robot, std::uint32_t blocks)
{
    const auto previous = robot.location();
    robot.move(blocks);
    if (m_grid.isOffGrid(robot.location()) || m_grid.isOccupied(robot.location()))
    {
        robot.setLocation(previous);
        return false;
    }
    m_grid.updateLocation(previous, robot);
    return true;
}

bool RobotSimulator::erase(RobotFactory::Robot &robot)
{
    const auto name = canonicalName(robot.model());
    m_grid.remove(robot);
    m_robots_by_id.erase(robot.id());
    return m_robots_by_name.erase(name) == 1;
}

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
    if (key.empty() || m_robots_by_name.contains(key))
    {
        return false;
    }

    auto robot = RobotFactory::RobotAssembly::create(type, location, key);
    if (!robot || !m_grid.addRobot(*robot))
    {
        return false;
    }
    auto *raw_robot = robot.get();
    m_robots_by_id.emplace(robot->id(), raw_robot);
    m_robots_by_name.emplace(key, std::move(robot));
    return true;
}

bool RobotSimulator::move(std::string_view name, std::uint32_t blocks)
{
    auto *robot = find(name);
    return robot != nullptr && move(*robot, blocks);
}

bool RobotSimulator::move(RobotFactory::RobotId id, std::uint32_t blocks)
{
    auto *robot = find(id);
    return robot != nullptr && move(*robot, blocks);
}

std::size_t RobotSimulator::moveAll(std::uint32_t blocks)
{
    std::size_t moved{0};
    for (auto &[name, robot] : m_robots_by_name)
    {
        static_cast<void>(name);
        moved += move(*robot, blocks) ? 1U : 0U;
    }
    return moved;
}

bool RobotSimulator::rotate(std::string_view name, RobotFactory::Rotation rotation)
{
    auto *robot = find(name);
    if (robot == nullptr)
    {
        return false;
    }
    robot->rotate(rotation);
    return true;
}

bool RobotSimulator::rotate(RobotFactory::RobotId id, RobotFactory::Rotation rotation)
{
    auto *robot = find(id);
    if (robot == nullptr)
    {
        return false;
    }
    robot->rotate(rotation);
    return true;
}

std::size_t RobotSimulator::rotateAll(RobotFactory::Rotation rotation)
{
    for (auto &[name, robot] : m_robots_by_name)
    {
        static_cast<void>(name);
        robot->rotate(rotation);
    }
    return m_robots_by_name.size();
}

bool RobotSimulator::remove(std::string_view name)
{
    auto *robot = find(name);
    return robot != nullptr && erase(*robot);
}

bool RobotSimulator::remove(RobotFactory::RobotId id)
{
    auto *robot = find(id);
    return robot != nullptr && erase(*robot);
}

std::size_t RobotSimulator::removeAll()
{
    const auto count = m_robots_by_name.size();
    m_robots_by_id.clear();
    m_robots_by_name.clear();
    m_grid = RobotGrid{m_grid.size()};
    return count;
}

bool RobotSimulator::resize(GridSize size)
{
    return m_grid.resize(size);
}

void RobotSimulator::report(std::ostream &output) const
{
    const auto size = m_grid.size();
    output << "Grid: " << size.width << 'x' << size.height
           << "\nRobots: " << m_robots_by_name.size() << '\n';
    for (const auto &[name, robot] : m_robots_by_name)
    {
        static_cast<void>(name);
        Menu::showDetails(*robot, output);
    }
}

const RobotFactory::Robot *RobotSimulator::findRobot(std::string_view name) const
{
    return find(name);
}

const RobotFactory::Robot *RobotSimulator::findRobot(RobotFactory::RobotId id) const
{
    return find(id);
}

GridSize RobotSimulator::gridSize() const noexcept
{
    return m_grid.size();
}

std::size_t RobotSimulator::robotCount() const noexcept
{
    return m_robots_by_name.size();
}

} // namespace Simulator
