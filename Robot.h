#ifndef ROBOT_H
#define ROBOT_H

#include <ostream>
#include <string>
#include <string_view>

namespace RobotFactory
{

enum class Direction : size_t
{
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
};

[[nodiscard]] constexpr std::string_view toString(Direction direction) noexcept
{
    switch (direction)
    {
    case Direction::NORTH:
        return "NORTH";
    case Direction::EAST:
        return "EAST";
    case Direction::SOUTH:
        return "SOUTH";
    case Direction::WEST:
        return "WEST";
    }

    return "NORTH";
}

inline std::ostream &operator<<(std::ostream &stream, Direction direction)
{
    return stream << toString(direction);
}

[[nodiscard]] inline Direction directionFromString(std::string_view direction) noexcept
{
    if (direction == "EAST")
        return Direction::EAST;
    if (direction == "SOUTH")
        return Direction::SOUTH;
    if (direction == "WEST")
        return Direction::WEST;
    return Direction::NORTH;
}

struct RobotLocation
{
    RobotLocation() = default;
    RobotLocation(size_t x, size_t y, Direction heading) noexcept
        : x_coordinate{x}, y_coordinate{y}, direction{heading}
    {
    }
    RobotLocation(size_t x, size_t y, std::string_view heading) noexcept
        : RobotLocation{x, y, directionFromString(heading)}
    {
    }

    size_t x_coordinate{0};
    size_t y_coordinate{0};
    Direction direction{Direction::NORTH};
};

enum class ROBOT_ROTATION : size_t
{
    LEFT = 0,
    RIGHT
};

/**
 * @brief A Robot abstract class that provides a typical robot interface.
 */
class Robot
{
  public:
    Robot(std::string name) : m_location{}, m_model{std::move(name)}, m_robotID{++m_serial_number}
    {
        // TODO: Use random generated number as robot id
    }

    Robot(const RobotLocation &location) noexcept
        : m_location{location}, m_robotID{++m_serial_number}
    {
        // TODO: Use random generated number as robot id
    }

    Robot(const RobotLocation &location, std::string name)
        : m_location{location}, m_model{std::move(name)}, m_robotID{++m_serial_number}
    {
    }

    virtual ~Robot() = default;

    size_t Id() const noexcept
    {
        return m_robotID;
    }

    const std::string &model() const noexcept
    {
        return m_model;
    }

    RobotLocation location() const noexcept
    {
        return m_location;
    }

    void setLocation(const RobotLocation &location) noexcept
    {
        m_location = location;
    }

    void setModel(const std::string &model)
    {
        m_model = model;
    }

    virtual void rotate(const ROBOT_ROTATION &) = 0;
    virtual void move(size_t) = 0;

  protected:
    RobotLocation m_location;
    std::string m_model;
    size_t m_robotID;
    static size_t m_serial_number;
};
} // namespace RobotFactory

#endif
