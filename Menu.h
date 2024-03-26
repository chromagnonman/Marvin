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
        std::cout << "\nUsage: PLACE X,Y, Direction (NORTH, SOUTH, EAST, "
                    "WEST) Name (optional) i.e. PLACE 2,2, NORTH, R2D2";
        std::cout << "\n       MOVE";
        std::cout << "\n       LEFT";
        std::cout << "\n       RIGHT";
        std::cout << "\n       REPORT";
        std::cout << "\n       REMOVE (Robot ID/All)";
        std::cout << "\nCtrl+C to quit";
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

