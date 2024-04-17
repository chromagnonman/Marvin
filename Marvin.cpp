#include "Marvin.h"

namespace RobotFactory {

    using namespace RobotFactory::ROBOT_DIRECTION;

    size_t Robot::m_serial_number{42};

    Marvin::Marvin(const std::string& name) noexcept : Robot {name} 
    {
    }

    Marvin::Marvin(const RobotLocation& location) noexcept :
        Robot{ location }
    {
    }

    Marvin::Marvin(const RobotLocation& location, std::string robot_name) noexcept :
        Robot{ location, std::move(robot_name)}
    {
    }

    Marvin::~Marvin() = default;

    void Marvin::move(size_t unit) noexcept
    {
        if (m_location.direction == NORTH)
        {
            m_location.y_coordinate += (m_default_move * unit);
        }
        else if (m_location.direction == SOUTH)
        {
            m_location.y_coordinate -= (m_default_move * unit);
        }
        else if (m_location.direction == EAST)
        {
            m_location.x_coordinate += (m_default_move * unit);
        }
        else if (m_location.direction == WEST)
        {
            m_location.x_coordinate -= (m_default_move * unit);
        }
    }

    void Marvin::rotate(const ROBOT_ROTATION& rotate_direction) noexcept
    {
        if (m_location.direction == NORTH)
        {
            m_location.direction = (rotate_direction == ROBOT_ROTATION::LEFT) ? WEST : EAST;
        }
        else if (m_location.direction == SOUTH)
        {
            m_location.direction = (rotate_direction == ROBOT_ROTATION::LEFT) ? EAST : WEST;
        }
        else if (m_location.direction == EAST)
        {
            m_location.direction = (rotate_direction == ROBOT_ROTATION::LEFT) ? NORTH : SOUTH;
        }
        else if (m_location.direction == WEST)
        {
            m_location.direction = (rotate_direction == ROBOT_ROTATION::LEFT) ?  SOUTH : NORTH;
        }
    }
}

