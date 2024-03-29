#ifndef MENU_H
#define MENU_H

#include <memory>
#include <iostream>

namespace Simulator {

class Menu {
	public:

    static void showUsage() 
    {
        std::cout << "\nUsage: PLACE <Robot> (X,Y) <Direction>";
        std::cout << "\n\nExample: PLACE R2D2 2,2 NORTH";
        std::cout << "\n\n  COMMANDS:";
        std::cout << "\n\n    PLACE  (Robot into the Grid(10x10)";
        std::cout << "\n\n    MOVE   (Particular robot or all robots if no name is provided) Ex: MOVE R2D2 <number of paces>";
        std::cout << "\n    LEFT   (Rotates robot to the left; all robots will shift if no name is provided)";
        std::cout << "\n    RIGHT  (Rotates robot to the right; all robots will shift if no name is provided)";
        std::cout << "\n    REMOVE (Particular robot or all robots if no name is provided)";
        std::cout << "\n\n    REPORT (Shows all robots)";
        std::cout << "\n    MENU   (Shows this menu)";
        std::cout << "\n\n    Ctrl + C to quit";
        std::cout << "\n\n> ";
    }

    static void showDetails(const std::unique_ptr<RobotFactory::Robot>& robot) 
    {
        std::cout << "\nName: " << robot->name();

        std::cout << "\nlocation (" << robot->location().x_coordinate << ","
                  << robot->location().y_coordinate << "),"
                  << " facing " << robot->location().direction << '\n';
    }
};
}

#endif

