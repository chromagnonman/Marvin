#include "RobotGrid.h"

#include "Robot.h"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Simulator
{

namespace
{
[[nodiscard]] std::size_t cellCount(GridSize size)
{
    if (size.width <= 0 || size.height <= 0)
    {
        throw std::invalid_argument{"Grid dimensions must be positive."};
    }
    const auto width = static_cast<std::size_t>(size.width);
    const auto height = static_cast<std::size_t>(size.height);
    if (width > std::numeric_limits<std::size_t>::max() / height)
    {
        throw std::length_error{"Grid dimensions are too large."};
    }
    return width * height;
}
} // namespace

RobotGrid::RobotGrid() : RobotGrid{default_grid_size} {}

RobotGrid::RobotGrid(GridSize size) : m_cells(cellCount(size)), m_size{size} {}

bool RobotGrid::addRobot(const RobotFactory::Robot &robot)
{
    const auto location = robot.location();
    if (isOffGrid(location) || isOccupied(location))
    {
        return false;
    }
    m_cells.at(index(location)) = robot.id();
    return true;
}

void RobotGrid::updateLocation(RobotFactory::RobotLocation previous,
                               const RobotFactory::Robot &robot)
{
    m_cells.at(index(previous)) = 0;
    m_cells.at(index(robot.location())) = robot.id();
}

bool RobotGrid::resize(GridSize size)
{
    if (size.width < m_size.width || size.height < m_size.height)
    {
        return false;
    }

    std::vector<Cell> resized(cellCount(size));
    for (RobotFactory::Coordinate y = 0; y < m_size.height; ++y)
    {
        const auto old_offset = static_cast<std::size_t>(y * m_size.width);
        const auto new_offset = static_cast<std::size_t>(y * size.width);
        std::copy_n(m_cells.begin() + static_cast<std::ptrdiff_t>(old_offset),
                    static_cast<std::size_t>(m_size.width),
                    resized.begin() + static_cast<std::ptrdiff_t>(new_offset));
    }
    m_cells = std::move(resized);
    m_size = size;
    return true;
}

void RobotGrid::remove(const RobotFactory::Robot &robot)
{
    const auto location = robot.location();
    if (!isOffGrid(location))
    {
        m_cells.at(index(location)) = 0;
    }
}

GridSize RobotGrid::size() const noexcept
{
    return m_size;
}

RobotFactory::RobotId RobotGrid::robotIdAt(RobotFactory::RobotLocation location) const
{
    return m_cells.at(index(location));
}

bool RobotGrid::isOffGrid(RobotFactory::RobotLocation location) const noexcept
{
    return location.x < 0 || location.y < 0 || location.x >= m_size.width ||
           location.y >= m_size.height;
}

bool RobotGrid::isOccupied(RobotFactory::RobotLocation location) const
{
    return !isOffGrid(location) && m_cells.at(index(location)) != 0;
}

std::size_t RobotGrid::index(RobotFactory::RobotLocation location) const
{
    if (isOffGrid(location))
    {
        throw std::out_of_range{"Grid location is outside the grid."};
    }
    return static_cast<std::size_t>((location.y * m_size.width) + location.x);
}

} // namespace Simulator
