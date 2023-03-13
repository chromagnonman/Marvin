#pragma once

#include <string>
#include <unordered_set>

namespace RobotFactory
{
	
	struct RobotLocation {

		/*enum class DIRECTION : size_t {
			SOUTH = 0,
			WEST,
			EAST,
			NORTH
		};*/

		size_t x_coordinate;
		size_t y_coordinate;
		std::string direction;
	};

	enum class ROBOT_ROTATION : size_t {
		LEFT = 0,
		RIGHT
	};

	static std::unordered_set<std::string> direction{ "NORTH", "SOUTH", "EAST", "WEST" };

	/**
	* @brief A Robot abstract class that provides a typical robot interface.
	*/
	class Robot {
	public:
		Robot() noexcept
		{
			++m_robotID;
		}

		Robot(const RobotLocation& location) noexcept : 
			m_location{location}
		{ 
			++m_robotID; 
		}

		Robot(const RobotLocation& location, std::string&& name) noexcept : 
			m_location{location},
			m_name{ name }
		{
			++m_robotID;
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
		std::string m_name{ "R2D2" };
		size_t m_robotID{ 000 };
	};
}