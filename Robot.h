#pragma once

#include <string>

namespace RobotFactory
{
	enum class ROTATE_DIRECTION : size_t {
		LEFT = 0,
		RIGHT
	};

	/**
	* @brief A Robot abstract class that provides a typical robot interface.
	*/
	class Robot {
	public:
		virtual ~Robot() = default;

		virtual void rotate(const ROTATE_DIRECTION& rotate_direction = ROTATE_DIRECTION::LEFT) = 0;
		virtual void move() = 0;
	};
}