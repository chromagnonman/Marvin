#ifndef ROBOTSIMULATOR_H
#define ROBOTSIMULATOR_H

#include <memory>

#include "RobotAssembly.h"

namespace Simulator
{

class RobotSimulator
{
  public:
    RobotSimulator();
    explicit RobotSimulator(GridSize grid);
    virtual ~RobotSimulator();

    void start();

    bool place(const RobotFactory::ROBOT_TYPE &, const RobotFactory::RobotLocation &,
               const std::string &name);
    void move();
    void rotate(const std::string &direction);
    void remove();
    void report() const;
    void resize(GridSize);

    // Individual robot commands
    bool move(const std::string &robot, size_t blocks = 1);
    bool rotate(const std::string &robot, const std::string &direction);
    bool remove(const std::string &robot);

  private:
    struct impl;
    std::unique_ptr<impl> m_pImpl;
};
} // namespace Simulator

#endif
