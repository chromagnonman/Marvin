#include "marvin/simulator/Menu.h"

#include "marvin/robot/Robot.h"

#include <ostream>

namespace Simulator
{

void Menu::showUsage(std::ostream &output)
{
    output << "\nCommands (case-insensitive, default grid 10x10):\n"
              "  PLACE <name> <x>,<y> <NORTH|EAST|SOUTH|WEST>\n"
              "  MOVE [ALL|name|@id] [blocks]\n"
              "  ROTATE [ALL|name|@id] <LEFT|RIGHT>\n"
              "  LEFT|RIGHT [ALL|name|@id]\n"
              "  REMOVE [ALL|name|@id]\n"
              "  REPORT\n"
              "  RESIZE <width> <height>\n"
              "  MENU\n"
              "  QUIT\n\n> ";
}

void Menu::showDetails(const RobotFactory::Robot &robot, std::ostream &output)
{
    const auto location = robot.location();
    output << "\nName: " << robot.model() << "\nID: " << robot.id() << "\nLocation: (" << location.x
           << ',' << location.y << "), facing " << location.direction << '\n';
}

} // namespace Simulator
