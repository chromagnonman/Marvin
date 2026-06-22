#ifndef MENU_H
#define MENU_H

#include "marvin/robot/Robot.h"

#include <ostream>

namespace Simulator
{

struct Menu
{
    static void showUsage(std::ostream &output);
    static void showDetails(const RobotFactory::Robot &robot, std::ostream &output);
};

} // namespace Simulator

#endif
