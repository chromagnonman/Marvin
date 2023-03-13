#include "Marvin.h"
#include <iostream>

namespace RobotFactory {

	Marvin::Marvin(const RobotLocation& location) noexcept :
		Robot{ location }
	{
	}

	Marvin::Marvin(const RobotLocation& location, std::string name) noexcept :
		Robot{ location, std::move(name) }
	{
	}

	Marvin::Marvin(const Marvin& marvin) noexcept
	{
		if (this != &marvin)
		{
			m_location = marvin.m_location;
			m_robotID = marvin.m_robotID;
			m_name = marvin.m_name;
		}
	}

	Marvin& Marvin::operator=(const Marvin& marvin) noexcept
	{
		if (this != &marvin)
		{
			m_location = marvin.m_location;
			m_robotID = marvin.m_robotID;
			m_name = marvin.m_name;
		}

		return *this;
	}

	Marvin::~Marvin() = default;

	void Marvin::move() noexcept
	{
		if (m_location.direction == "NORTH")
		{
			m_location.y_coordinate++;
		}
		else if (m_location.direction == "SOUTH")
		{
			m_location.y_coordinate--;
		}
		else if (m_location.direction == "EAST")
		{
			m_location.x_coordinate++;
		}
		else if (m_location.direction == "WEST")
		{
			m_location.x_coordinate--;
		}
	}

	void Marvin::rotate(const ROBOT_ROTATION& rotate_direction) noexcept
	{
		if (m_location.direction == "NORTH") 
		{
			(rotate_direction == ROBOT_ROTATION::LEFT) ? m_location.direction = "WEST" : m_location.direction = "EAST";
		}
		else if (m_location.direction == "SOUTH") 
		{
			(rotate_direction == ROBOT_ROTATION::LEFT) ? m_location.direction = "EAST" : m_location.direction = "WEST";
		}
		else if (m_location.direction == "EAST")
		{
			(rotate_direction == ROBOT_ROTATION::LEFT) ? m_location.direction = "NORTH" : m_location.direction = "SOUTH";
		}
		else if (m_location.direction == "WEST")
		{
			(rotate_direction == ROBOT_ROTATION::LEFT) ? m_location.direction = "SOUTH" : m_location.direction = "NORTH";
		}
	}
}

