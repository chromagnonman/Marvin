#include "Robot.h"

#include <atomic>
#include <ostream>
#include <string>
#include <utility>

namespace RobotFactory
{

std::atomic<RobotId> Robot::s_next_id{42};

std::ostream &operator<<(std::ostream &stream, Direction direction)
{
    return stream << toString(direction);
}

Robot::Robot(RobotLocation location, std::string model)
    : m_location{location}, m_model{std::move(model)}, m_id{s_next_id.fetch_add(1) + 1}
{
}

RobotId Robot::id() const noexcept
{
    return m_id;
}

const std::string &Robot::model() const noexcept
{
    return m_model;
}

RobotLocation Robot::location() const noexcept
{
    return m_location;
}

void Robot::setLocation(RobotLocation location) noexcept
{
    m_location = location;
}

} // namespace RobotFactory
