#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "Robot.h"
#include "RobotGrid.h"

namespace Simulator
{

class RobotGrid::impl
{
  public:
    explicit impl(GridSize gridSz);

    bool addRobot(const RobotFactory::Robot &robot);

    void updateLocation(const RobotFactory::RobotLocation &location,
                        const RobotFactory::Robot &robot);

    [[nodiscard]] const GridSize &getSize() const noexcept;

    void resize(GridSize gridSz);

    void remove(const RobotFactory::Robot &robot);

    [[nodiscard]] size_t getRobotID(const RobotFactory::RobotLocation &location) const;
    [[nodiscard]] bool isOffTheGrid(const RobotFactory::Robot &robot) const noexcept;
    [[nodiscard]] bool isOccupied(const RobotFactory::Robot &robot) const;

  private:
    using ROBOT_ID = size_t;
    std::vector<std::vector<ROBOT_ID>> m_grid;
    GridSize m_gridSz;

    [[nodiscard]] ROBOT_ID &cell(const RobotFactory::RobotLocation &location);
    [[nodiscard]] const ROBOT_ID &cell(const RobotFactory::RobotLocation &location) const;
};

void RobotGrid::impl::resize(GridSize gridSz)
{
    std::vector<std::vector<ROBOT_ID>> resized(gridSz.height, std::vector<ROBOT_ID>(gridSz.width));

    const auto rows = std::min(m_gridSz.height, gridSz.height);
    const auto columns = std::min(m_gridSz.width, gridSz.width);
    for (size_t y = 0; y < rows; ++y)
    {
        std::copy_n(m_grid.at(y).begin(), columns, resized.at(y).begin());
    }

    m_grid = std::move(resized);
    m_gridSz = gridSz;
}

RobotGrid::impl::ROBOT_ID &RobotGrid::impl::cell(const RobotFactory::RobotLocation &location)
{
    return m_grid.at(location.y_coordinate).at(location.x_coordinate);
}

const RobotGrid::impl::ROBOT_ID &RobotGrid::impl::cell(
    const RobotFactory::RobotLocation &location) const
{
    return m_grid.at(location.y_coordinate).at(location.x_coordinate);
}

RobotGrid::impl::impl(GridSize gridSz)
    : m_grid(gridSz.height, std::vector<ROBOT_ID>(gridSz.width)), m_gridSz{gridSz}
{
}

bool RobotGrid::impl::addRobot(const RobotFactory::Robot &robot)
{
    if (!isOffTheGrid(robot) && !isOccupied(robot))
    {
        cell(robot.location()) = robot.Id();

        return true;
    }

    return false;
}

void RobotGrid::impl::updateLocation(const RobotFactory::RobotLocation &location,
                                     const RobotFactory::Robot &robot)
{
    cell(location) = 0;
    cell(robot.location()) = robot.Id();
}

size_t RobotGrid::impl::getRobotID(const RobotFactory::RobotLocation &location) const
{
    return cell(location);
}

const GridSize &RobotGrid::impl::getSize() const noexcept
{
    return m_gridSz;
}

bool RobotGrid::impl::isOffTheGrid(const RobotFactory::Robot &robot) const noexcept
{
    return robot.location().x_coordinate >= m_gridSz.width ||
           robot.location().y_coordinate >= m_gridSz.height;
}

bool RobotGrid::impl::isOccupied(const RobotFactory::Robot &robot) const
{
    return !isOffTheGrid(robot) && cell(robot.location()) != 0;
}

void RobotGrid::impl::remove(const RobotFactory::Robot &robot)
{
    if (!isOffTheGrid(robot))
    {
        cell(robot.location()) = 0;
    }
}

RobotGrid::RobotGrid(GridSize grid) : m_pImpl{std::make_unique<impl>(grid)} {}

RobotGrid::RobotGrid()
    : m_pImpl{std::make_unique<impl>(GridSize{.width = DEFAULT_WIDTH, .height = DEFAULT_HEIGHT})}
{
}

RobotGrid::~RobotGrid() noexcept = default;

bool RobotGrid::addRobot(const RobotFactory::Robot &robot)
{
    return m_pImpl->addRobot(robot);
}

void RobotGrid::updateLocation(const RobotFactory::RobotLocation &location,
                               const RobotFactory::Robot &robot)
{
    m_pImpl->updateLocation(location, robot);
}

const GridSize &RobotGrid::getSize() const noexcept
{
    return m_pImpl->getSize();
}

void RobotGrid::resize(GridSize grid)
{
    m_pImpl->resize(grid);
}

void RobotGrid::remove(const RobotFactory::Robot &robot)
{
    m_pImpl->remove(robot);
}

size_t RobotGrid::getRobotID(const RobotFactory::RobotLocation &location) const
{
    return m_pImpl->getRobotID(location);
}

bool RobotGrid::isOffTheGrid(const RobotFactory::Robot &robot) const noexcept
{
    return m_pImpl->isOffTheGrid(robot);
}

bool RobotGrid::isOccupied(const RobotFactory::Robot &robot) const
{
    return m_pImpl->isOccupied(robot);
}

} // namespace Simulator
