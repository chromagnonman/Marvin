#pragma once

#include "Robot.h"
#include "RobotGrid.h"
#include <memory>

namespace RobotFactory {

	/**
	* @brief Defines a type called Marvin - a chronically depressed robot.
	*/

	class Marvin : public Robot {
	public:
		Marvin(const RobotLocation& location) noexcept;
		Marvin(const RobotLocation&, std::string name) noexcept;
		Marvin(const Marvin&) noexcept;
		Marvin& operator=(const Marvin&) noexcept;
		virtual ~Marvin();

		void rotate(const ROBOT_ROTATION& rotate_direction = ROBOT_ROTATION::LEFT) noexcept override;
		void move() noexcept override;
	};
}