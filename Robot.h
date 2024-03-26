#pragma once

#include <string>

namespace RobotFactory
{
    
    struct RobotLocation {
        int x_coordinate;
        int y_coordinate;
        std::string direction;
    };

    enum class ROBOT_ROTATION : size_t {
        LEFT = 0,
        RIGHT
    };

    namespace ROBOT_DIRECTION {
        static constexpr auto NORTH = "NORTH";
        static constexpr auto SOUTH = "SOUTH";
        static constexpr auto EAST = "EAST";
        static constexpr auto WEST = "WEST";
    }

    /**
    * @brief A Robot abstract class that provides a typical robot interface.
    */
    class Robot {
    public:
        Robot() noexcept : m_location{}, m_robotID{++m_ID}
        {
        }

        Robot(const RobotLocation& location) noexcept : 
            m_location{location}, m_robotID{++m_ID}
        { 
        }

        Robot(const RobotLocation& location, std::string&& name) noexcept : 
            m_location{location}, m_name{name}, m_robotID{++m_ID}
        {
        }

        virtual ~Robot() = default;

        size_t Id() const noexcept
        {
            return m_robotID;
        }

        const std::string& name() const noexcept
        {
            return m_name;
        }

        RobotLocation location() const noexcept
        {
            return m_location;
        }

        void setLocation(const RobotLocation& location) noexcept
        {
            m_location = location;
        }

        virtual void rotate(const ROBOT_ROTATION& rotate_direction = ROBOT_ROTATION::LEFT) = 0;
        virtual void move() = 0;

    protected:
        RobotLocation m_location;
        std::string m_name{ "Marvin" };
        size_t m_robotID;
        static size_t m_ID;
    };
}