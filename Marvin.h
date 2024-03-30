#ifndef MARVIN_H
#define MARVIN_H

#include "Robot.h"
#include "RobotGrid.h"

namespace RobotFactory {

	/**
	* @brief Defines a type called Marvin - a chronically depressed robot.
	*/
	class Marvin : public Robot {
	public:
        Marvin();
		Marvin(const RobotLocation& location) noexcept;
		Marvin(const RobotLocation&, std::string robot_name) noexcept;
		Marvin(const Marvin&) noexcept;
		Marvin& operator=(const Marvin&) noexcept;
		virtual ~Marvin();

		void rotate(const ROBOT_ROTATION& rotate_direction = ROBOT_ROTATION::LEFT) noexcept override;
		void move(size_t pace=1) noexcept override;
	};
}

#endif
