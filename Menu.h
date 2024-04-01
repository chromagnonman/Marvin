#ifndef MENU_H
#define MENU_H

#include <memory>
#include <iostream>

namespace Simulator {

class Menu {
	public:

    static void showUsage() 
    {
        std::cout << "\n\n  COMMANDS (case insensitive):";
        std::cout << "\n\n    Place  (Place <robot> <x,y> <north,south,west,east>) e.g. Place R2D2 1,1 north";
        std::cout << "\n\n    Move   (Move <robot> <number of units> or just MOVE, which moves all the robots)";
        std::cout << "\n    Rotate (Rotate <robot> <left/right> or just ROTATE)";
        std::cout << "\n    Remove (Remove <robot> or just REMOVE)";
        std::cout << "\n\n    Report (Shows all robots)";
        std::cout << "\n    Menu   ";
        std::cout << "\n\n    Ctrl + C to quit";
        std::cout << "\n\n> ";
    }

    static void showDetails(const std::unique_ptr<RobotFactory::Robot>& robot) 
    {
        std::cout << "\nName: " << robot->model();
        std::cout << "\nID: " << robot->Id();
        std::cout << "\nlocation (" << robot->location().x_coordinate << ","
                  << robot->location().y_coordinate << "),"
                  << " facing " << robot->location().direction << '\n';
    }
};
}

#endif

