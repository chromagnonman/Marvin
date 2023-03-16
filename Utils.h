#pragma once

#include <iostream>

#include "Robot.h"
#include "RobotSimulator.h"

namespace Utils {

	volatile std::sig_atomic_t signal_status;

	void signal_handler(int signal)
	{
		signal_status = signal;
	}

	std::string getDirection(const std::string& direction) noexcept
	{
		using namespace RobotFactory::ROBOT_DIRECTION;
		if (_stricmp(direction.c_str(), NORTH) == 0)
		{
			return NORTH;
		}
		else if (_stricmp(direction.c_str(), SOUTH) == 0)
		{
			return SOUTH;
		}
		else if (_stricmp(direction.c_str(), EAST) == 0)
		{
			return EAST;
		}
		else if (_stricmp(direction.c_str(), WEST) == 0)
		{
			return WEST;
		}

		return NORTH;
	}

	void showMenu() noexcept
	{	
		std::cout << "\nConstraints: X and Y should not be greater than or equal to ten.";
		std::cout << "\nUsage: PLACE X,Y, Direction (NORTH, SOUTH, EAST, WEST)";
		std::cout << "\n       MOVE";
		std::cout << "\n       LEFT";
		std::cout << "\n       RIGHT";
		std::cout << "\n       REPORT";
		std::cout << "\n\n> ";
	}

	void showReport(const std::shared_ptr<RobotFactory::Robot>& robot) noexcept
	{
		std::cout << "\nRobot ID: " << robot->Id();
		std::cout << "\nRobot name: " << robot->name();
		std::cout << "\nRobot location (" << robot->location().x_coordinate << "," << robot->location().y_coordinate
			<< ")," << robot->location().direction << '\n';
	}
}