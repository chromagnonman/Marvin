#ifndef ROBOT_GRID_H
#define ROBOT_GRID_H

#include "Robot.h"

#include <vector>

namespace Simulator
{

struct GridSize
{
    RobotFactory::Coordinate width{10};
    RobotFactory::Coordinate height{10};
};

inline constexpr GridSize default_grid_size{};

class RobotGrid
{
  public:
    RobotGrid();
    explicit RobotGrid(GridSize size);

    [[nodiscard]] bool addRobot(const RobotFactory::Robot &robot);
    void updateLocation(RobotFactory::RobotLocation previous, const RobotFactory::Robot &robot);
    [[nodiscard]] bool resize(GridSize size);
    void remove(const RobotFactory::Robot &robot);

    [[nodiscard]] GridSize size() const noexcept;
    [[nodiscard]] RobotFactory::RobotId robotIdAt(RobotFactory::RobotLocation location) const;
    [[nodiscard]] bool isOffGrid(RobotFactory::RobotLocation location) const noexcept;
    [[nodiscard]] bool isOccupied(RobotFactory::RobotLocation location) const;

  private:
    using Cell = RobotFactory::RobotId;
    std::vector<Cell> m_cells;
    GridSize m_size;

    [[nodiscard]] std::size_t index(RobotFactory::RobotLocation location) const;
};

} // namespace Simulator

#endif
