#ifndef ROBOT_SIMULATOR_H
#define ROBOT_SIMULATOR_H

#include "marvin/command/Command.h"
#include "marvin/robot/Robot.h"
#include "marvin/robot/RobotAssembly.h"
#include "marvin/simulator/RobotGrid.h"

#include <cstdint>
#include <iosfwd>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Simulator
{

class RobotSimulator
{
  public:
    RobotSimulator();
    explicit RobotSimulator(GridSize size);

    RobotSimulator(const RobotSimulator &) = delete;
    RobotSimulator &operator=(const RobotSimulator &) = delete;
    RobotSimulator(RobotSimulator &&) noexcept = default;
    RobotSimulator &operator=(RobotSimulator &&) noexcept = default;

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
    RobotGrid m_grid;
    std::unordered_map<std::string, std::unique_ptr<RobotFactory::Robot>> m_robots_by_name;
    std::unordered_map<RobotFactory::RobotId, RobotFactory::Robot *> m_robots_by_id;

    [[nodiscard]] RobotFactory::Robot *find(std::string_view name);
    [[nodiscard]] const RobotFactory::Robot *find(std::string_view name) const;
    [[nodiscard]] RobotFactory::Robot *find(RobotFactory::RobotId id);
    [[nodiscard]] const RobotFactory::Robot *find(RobotFactory::RobotId id) const;
    [[nodiscard]] bool move(RobotFactory::Robot &robot, std::uint32_t blocks);
    [[nodiscard]] bool erase(RobotFactory::Robot &robot);
};

} // namespace Simulator

#endif
