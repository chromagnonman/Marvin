#include "Marvin.h"
#include <array>

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
    // Rotation table: [current_direction][rotation_type] = new_direction
    static constexpr std::array<std::array<Direction, 2>, 4> rotation_table{
        {// NORTH
         {{Direction::WEST, Direction::EAST}},
         // EAST
         {{Direction::NORTH, Direction::SOUTH}},
         // SOUTH
         {{Direction::EAST, Direction::WEST}},
         // WEST
         {{Direction::SOUTH, Direction::NORTH}}}};

    const auto current_dir_idx = static_cast<size_t>(m_location.direction);
    const auto rotation_idx = static_cast<size_t>(rotate_direction);
    m_location.direction = rotation_table[current_dir_idx][rotation_idx];
}
} // namespace RobotFactory
