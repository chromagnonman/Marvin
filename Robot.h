#ifndef ROBOT_H
#define ROBOT_H

#include <string>


namespace RobotFactory
{
    
    struct RobotLocation {
        size_t x_coordinate{0};
        size_t y_coordinate{0};
        std::string direction {"NORTH"};
    };

    enum class ROBOT_ROTATION : size_t {
        LEFT = 0,
        RIGHT
    };

    namespace ROBOT_DIRECTION {
        static constexpr auto NORTH = "NORTH";
        static constexpr auto SOUTH = "SOUTH";
        static constexpr auto EAST  = "EAST";
        static constexpr auto WEST  = "WEST";
    }

    /**
    * @brief A Robot abstract class that provides a typical robot interface.
    */
    class Robot {
    public:
        Robot() noexcept : m_location{}, m_robotID{++m_serial_number}
        {
            // TODO: Use random generated number as robot id
        }

        Robot(const RobotLocation& location) noexcept : 
            m_location{location}, m_robotID{++m_serial_number}
        {
            // TODO: Use random generated number as robot id
        }

        Robot(const RobotLocation& location, std::string name) noexcept
            : 
            m_location{location}, m_model{std::move(name)}, m_robotID{++m_serial_number}
        {
        }

        virtual ~Robot() = default;

        size_t Id() const noexcept
        {
            return m_robotID;
        }

        std::string model() const noexcept
        {
            return m_model;
        }

        RobotLocation location() const noexcept
        {
            return m_location;
        }

        void setLocation(const RobotLocation& location) noexcept
        {
            m_location = location;
        }

        void setModel(const std::string& model) noexcept 
        {
            m_model = model;
        }

        virtual void rotate(const ROBOT_ROTATION& rotate_direction = ROBOT_ROTATION::LEFT) = 0;
        virtual void move(size_t pace = 1) = 0;

    protected:
        RobotLocation m_location;
        std::string m_model;
        size_t m_robotID;
        static size_t m_serial_number;
    };
}

#endif
