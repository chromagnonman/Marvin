#pragma once

#include "Robot.h"
#include <memory>

namespace RobotFactory {

	/**
	* @brief Defines a type called Marvin - a chronically depressed robot.
	*/

	class Marvin : public Robot {
	public:
		Marvin() noexcept;
		Marvin(const Marvin&) noexcept;
		Marvin& operator=(const Marvin&) noexcept;
		virtual ~Marvin();

		void rotate(const ROTATE_DIRECTION& rotate_direction = ROTATE_DIRECTION::LEFT) noexcept override;
		void move() noexcept override;

	private:
		struct impl;
		std::shared_ptr<impl> m_pImpl;
	};
}