#include "marvin/robot/Marvin.h"
#include "marvin/robot/Robot.h"

#include <cstdint>
#include <string>
#include <utility>

namespace RobotFactory
{

Marvin::Marvin(std::string name) : Robot{{}, std::move(name)} {}

Marvin::Marvin(RobotLocation location, std::string name) : Robot{location, std::move(name)} {}

void Marvin::move(std::uint32_t blocks) noexcept
{
    const auto distance = static_cast<Coordinate>(blocks);
    switch (m_location.direction)
    {
    case Direction::North:
        m_location.y += distance;
        break;
    case Direction::East:
        m_location.x += distance;
        break;
    case Direction::South:
        m_location.y -= distance;
        break;
    case Direction::West:
        m_location.x -= distance;
        break;
    }
}

void Marvin::rotate(Rotation rotation) noexcept
{
    constexpr std::uint8_t direction_count{4};
    constexpr std::uint8_t left_offset{direction_count - 1};
    constexpr std::uint8_t right_offset{1};
    const auto offset = rotation == Rotation::Left ? left_offset : right_offset;
    const auto direction =
        (static_cast<std::uint8_t>(m_location.direction) + offset) % direction_count;
    m_location.direction = static_cast<Direction>(direction);
}

} // namespace RobotFactory
