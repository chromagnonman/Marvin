#ifndef MENU_H
#define MENU_H

#include <memory>
#include <iostream>

#include "Robot.h"

namespace RobotWorldSimulator {

class Menu {
	public:

    static void showUsage() 
    {
        std::cout << "\nExample: PLACE 2,2, NORTH, R2D2";
        std::cout << "\n\n    COMMANDS:";
        std::cout << "\n\n    PLACE";
        std::cout << "\n    MOVE   (ROBOT ID/All)";
        std::cout << "\n    LEFT   (ID/All)";
        std::cout << "\n    RIGHT  (ID/All)";
        std::cout << "\n    REPORT (ID/All)";
        std::cout << "\n    REMOVE (ID/All)";
        std::cout << "\n\n    MENU";
        std::cout << "\n\n    Ctrl+C to quit";
        std::cout << "\n\n> ";
    }

    static void showDetails(const std::unique_ptr<RobotFactory::Robot>& robot) 
    {
        std::cout << "\nRobot ID: " << robot->Id();
        std::cout << "\nRobot name: " << robot->name();
        std::cout << "\nRobot location (" << robot->location().x_coordinate << ","
        << robot->location().y_coordinate << "),"
        << robot->location().direction << '\n';
    }
};
}

#endif

