#ifndef ROBOT_SIMULATOR_H
#define ROBOT_SIMULATOR_H

#include "Command.h"
#include "Robot.h"
#include "RobotAssembly.h"
#include "RobotGrid.h"

#include <cstdint>
#include <iosfwd>
#include <memory>
#include <string>
#include <string_view>

namespace Simulator
{

class RobotSimulator
{
  public:
    RobotSimulator();
    explicit RobotSimulator(GridSize size);
    ~RobotSimulator();

    RobotSimulator(const RobotSimulator &) = delete;
    RobotSimulator &operator=(const RobotSimulator &) = delete;
    RobotSimulator(RobotSimulator &&) noexcept;
    RobotSimulator &operator=(RobotSimulator &&) noexcept;

    void start();
    void run(std::istream &input, std::ostream &output, std::ostream &errors);
    [[nodiscard]] bool executeLine(std::string_view line, std::ostream &output,
                                   std::ostream &errors);

    [[nodiscard]] bool place(RobotFactory::GroundRobotType type,
                             RobotFactory::RobotLocation location, std::string_view name);
    [[nodiscard]] bool move(std::string_view name, std::uint32_t blocks = 1);
    [[nodiscard]] bool move(RobotFactory::RobotId id, std::uint32_t blocks = 1);
    [[nodiscard]] std::size_t moveAll(std::uint32_t blocks = 1);
    [[nodiscard]] bool rotate(std::string_view name, RobotFactory::Rotation rotation);
    [[nodiscard]] bool rotate(RobotFactory::RobotId id, RobotFactory::Rotation rotation);
    [[nodiscard]] std::size_t rotateAll(RobotFactory::Rotation rotation);
    [[nodiscard]] bool remove(std::string_view name);
    [[nodiscard]] bool remove(RobotFactory::RobotId id);
    [[nodiscard]] std::size_t removeAll();
    [[nodiscard]] bool resize(GridSize size);
    void report(std::ostream &output) const;

    [[nodiscard]] const RobotFactory::Robot *findRobot(std::string_view name) const;
    [[nodiscard]] const RobotFactory::Robot *findRobot(RobotFactory::RobotId id) const;
    [[nodiscard]] GridSize gridSize() const noexcept;
    [[nodiscard]] std::size_t robotCount() const noexcept;

  private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace Simulator

#endif
