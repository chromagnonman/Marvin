#include "Marvin.h"
#include "Robot.h"

#include <cstddef>
#include <string>
#include <utility>

namespace RobotFactory
{

size_t Robot::m_serial_number{42};

Marvin::Marvin(const std::string &name) : Robot{name} {}

Marvin::Marvin(const RobotLocation &location) noexcept : Robot{location} {}

Marvin::Marvin(const RobotLocation &location, std::string robot_name)
    : Robot{location, std::move(robot_name)}
{
}

Marvin::~Marvin() = default;

void Marvin::move(size_t unit) noexcept
{
    const size_t distance = m_default_move * unit;

    switch (m_location.direction)
    {
    case Direction::NORTH:
        m_location.y_coordinate += distance;
        break;
    case Direction::SOUTH:
        m_location.y_coordinate -= distance;
        break;
    case Direction::EAST:
        m_location.x_coordinate += distance;
        break;
    case Direction::WEST:
        m_location.x_coordinate -= distance;
        break;
    }
}

void Marvin::rotate(const ROBOT_ROTATION &rotate_direction) noexcept
{
    constexpr size_t direction_count{4};
    constexpr size_t left_offset{direction_count - 1};
    constexpr size_t right_offset{1};
    const size_t offset = rotate_direction == ROBOT_ROTATION::LEFT ? left_offset : right_offset;
    const auto direction = (static_cast<size_t>(m_location.direction) + offset) % direction_count;
    m_location.direction = static_cast<Direction>(direction);
}
} // namespace RobotFactory
