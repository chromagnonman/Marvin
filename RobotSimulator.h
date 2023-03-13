#pragma once

#include "Robot.h"
#include "RobotGrid.h"
#include <memory>


/**
* @brief 
*/

namespace RobotWorldSimulator {

	class RobotSimulator {
	public:
		explicit RobotSimulator(RobotGrid& world) noexcept;
		virtual ~RobotSimulator();

		void start() noexcept;
		void place(const RobotFactory::RobotLocation& location) noexcept;
		void place(const RobotFactory::RobotLocation& location, const std::string& name) noexcept;
		void report() const noexcept;
		void move() noexcept;
		void rotateLeft() noexcept;
		void rotateRight() noexcept;
		void end() noexcept;

	private:
		struct impl;
		std::unique_ptr<impl> m_pImpl;
	};
}