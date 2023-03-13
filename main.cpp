// Marvin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <limits>
#include <string>
#include "Marvin.h"

int main()
{
  /*  std::cout << "do something: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string input;
    while (std::getline(std::cin, input));
    std::cout << '\n' << input;*/

    using RobotDirection = RobotFactory::RobotLocation::DIRECTION;

    RobotFactory::Marvin marvin({ 1,2, "RobotDirection::SOUTH" });
    marvin.move();
    marvin.rotate();
    marvin.rotate(RobotFactory::ROTATE_DIRECTION::RIGHT);
    std::cout << "Location(xy): " << marvin.location().x_coordinate << marvin.location().y_coordinate << marvin.currentDirection() << '\n';

    RobotFactory::Marvin m2(marvin);
    m2.move();
    std::cout << "Location(xy): " << m2.location().x_coordinate << m2.location().y_coordinate << m2.currentDirection() << '\n';

    RobotFactory::Marvin m3({ 0,0, "RobotDirection::NORTH" });
    m3 = m2;
    m3.move();
    std::cout << "Location(xy): " << m3.location().x_coordinate << m3.location().y_coordinate << m3.currentDirection() << '\n';

    RobotFactory::Marvin m4({ 1,1, "RobotDirection::NORTH" });
    m4 = std::move(RobotFactory::Marvin({ 0,0, "RobotDirection::NORTH" }));
    std::cout << "Location(xy): " << m4.location().x_coordinate << m4.location().y_coordinate << m4.currentDirection() << '\n';


    RobotFactory::Robot* robot = new RobotFactory::Marvin({ 2,1, "RobotDirection::WEST" });
    robot->move();
    robot->rotate(RobotFactory::ROTATE_DIRECTION::RIGHT);
    robot->rotate();
    std::cout << "Location(xy): " << robot->location().x_coordinate << robot->location().y_coordinate << robot->currentDirection();
    delete robot;

    auto robot2 = std::make_unique<RobotFactory::Marvin>(RobotFactory::RobotLocation{ 3, 3, "RobotDirection::NORTH" });
    robot2->move();
    std::cout << "Location(xy): " << robot2->location().x_coordinate << robot2->location().y_coordinate << robot2->currentDirection();

    


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
