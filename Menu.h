#ifndef MENU_H
#define MENU_H

#include <memory>
#include <iostream>

namespace Simulator {

struct Menu {

    static void showUsage() 
    {
        // TODO: Add choices of robot types
        std::cout << "\n\n  COMMANDS (case insensitive) (Default Grid size (10x10):";
        std::cout << "\n\n    Place  (Place <Robot> <X,Y> <North, South, West, East>) e.g. Place R2D2 1,1 North";
        std::cout << "\n\n    Move   (Move <Robot> <number of units> or Move All, which moves all the robots)";
        std::cout << "\n    Rotate (Rotate <Robot/All> <left/right> or Left/Right <Robot/All>; all robots will rotate if none is specified)";
        std::cout << "\n    Remove (Remove <Robot/All>; all robots will be removed if none is specified )";
        std::cout << "\n\n    Report (Shows the current grid size and all the occypying robots)";
        std::cout << "\n    Resize (Expands the current grid. i.e. Resize 20 20)";
        std::cout << "\n\n    Menu   ";
        std::cout << "\n\n    Ctrl + C to terminate application.";
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

