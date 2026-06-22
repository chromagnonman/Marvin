#ifndef ROBOT_H
#define ROBOT_H

#include <atomic>
#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>

namespace RobotFactory
{

using Coordinate = std::int64_t;
using RobotId = std::uint64_t;

enum class Direction : std::uint8_t
{
    North,
    East,
    South,
    West
};

enum class Rotation : std::uint8_t
{
    Left,
    Right
};

[[nodiscard]] constexpr std::string_view toString(Direction direction) noexcept
{
    switch (direction)
    {
    case Direction::North:
        return "NORTH";
    case Direction::East:
        return "EAST";
    case Direction::South:
        return "SOUTH";
    case Direction::West:
        return "WEST";
    }

    return "NORTH";
}

std::ostream &operator<<(std::ostream &stream, Direction direction);

struct RobotLocation
{
    Coordinate x{0};
    Coordinate y{0};
    Direction direction{Direction::North};
};

class Robot
{
  public:
    Robot(RobotLocation location, std::string model);
    virtual ~Robot() = default;

    Robot(const Robot &) = delete;
    Robot &operator=(const Robot &) = delete;
    Robot(Robot &&) = delete;
    Robot &operator=(Robot &&) = delete;

    [[nodiscard]] RobotId id() const noexcept;
    [[nodiscard]] const std::string &model() const noexcept;
    [[nodiscard]] RobotLocation location() const noexcept;
    void setLocation(RobotLocation location) noexcept;

    virtual void rotate(Rotation rotation) noexcept = 0;
    virtual void move(std::uint32_t blocks) noexcept = 0;

  protected:
    RobotLocation m_location;

  private:
    std::string m_model;
    RobotId m_id;
    static std::atomic<RobotId> s_next_id;
};

} // namespace RobotFactory

#endif
