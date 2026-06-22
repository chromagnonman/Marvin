#ifndef ROBOTGRID_H
#define ROBOTGRID_H

#include <memory>

#include "Robot.h"

namespace Simulator
{

struct GridSize
{
    size_t width;
    size_t height;
};

static constexpr size_t DEFAULT_WIDTH = 10;
static constexpr size_t DEFAULT_HEIGHT = 10;

/**
 * @brief Provides a 2D grid environment for Marvin (the paranoid android) to explore.
 */
class RobotGrid
{
  public:
    RobotGrid();
    explicit RobotGrid(GridSize grid);

    RobotGrid(const RobotGrid &) = delete;
    RobotGrid &operator=(const RobotGrid &) = delete;

    virtual ~RobotGrid() noexcept;

    bool addRobot(const RobotFactory::Robot &) noexcept;

    void updateLocation(const RobotFactory::RobotLocation &, const RobotFactory::Robot &) noexcept;

    void resize(GridSize);
    void remove(const RobotFactory::Robot &robot) noexcept;

    [[nodiscard]] const GridSize &getSize() const noexcept;
    [[nodiscard]] size_t getRobotID(const RobotFactory::RobotLocation &) const noexcept;
    [[nodiscard]] bool isOffTheGrid(const RobotFactory::Robot &) const noexcept;
    [[nodiscard]] bool isOccupied(const RobotFactory::Robot &) const noexcept;

  private:
    struct impl;
    std::unique_ptr<impl> m_pImpl;
};
} // namespace Simulator

#endif
