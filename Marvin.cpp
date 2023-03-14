#include "Marvin.h"
#include <iostream>

namespace RobotFactory {

	size_t Robot::m_ID{ 0 };

	Marvin::Marvin(const RobotLocation& location) noexcept :
		Robot{ location }
	{
	}

	Marvin::Marvin(const RobotLocation& location, std::string robot_name) noexcept :
		Robot{ location, std::move(robot_name) }
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
		if (m_location.direction == ROBOT_DIRECTION::NORTH)
		{
			m_location.y_coordinate++;
		}
		else if (m_location.direction == ROBOT_DIRECTION::SOUTH)
		{
			m_location.y_coordinate--;
		}
		else if (m_location.direction == ROBOT_DIRECTION::EAST)
		{
			m_location.x_coordinate++;
		}
		else if (m_location.direction == ROBOT_DIRECTION::WEST)
		{
			m_location.x_coordinate--;
		}
	}

	void Marvin::rotate(const ROBOT_ROTATION& rotate_direction) noexcept
	{
		if (m_location.direction == ROBOT_DIRECTION::NORTH)
		{
			(rotate_direction == ROBOT_ROTATION::LEFT) ? m_location.direction = ROBOT_DIRECTION::WEST : m_location.direction = ROBOT_DIRECTION::EAST;
		}
		else if (m_location.direction == ROBOT_DIRECTION::SOUTH)
		{
			(rotate_direction == ROBOT_ROTATION::LEFT) ? m_location.direction = ROBOT_DIRECTION::EAST : m_location.direction = ROBOT_DIRECTION::WEST;
		}
		else if (m_location.direction == ROBOT_DIRECTION::EAST)
		{
			(rotate_direction == ROBOT_ROTATION::LEFT) ? m_location.direction = ROBOT_DIRECTION::NORTH : m_location.direction = ROBOT_DIRECTION::SOUTH;
		}
		else if (m_location.direction == ROBOT_DIRECTION::WEST)
		{
			(rotate_direction == ROBOT_ROTATION::LEFT) ? m_location.direction = ROBOT_DIRECTION::SOUTH : m_location.direction = ROBOT_DIRECTION::NORTH;
		}
	}
}

