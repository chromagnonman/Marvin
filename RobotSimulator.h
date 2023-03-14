#pragma once

#include <memory>
#include <unordered_set>

#include "Robot.h"
#include "RobotGrid.h"

namespace RobotWorldSimulator {

	namespace COMMAND {
		static constexpr auto PLACE = "PLACE";
		static constexpr auto MOVE = "MOVE";
		static constexpr auto LEFT = "LEFT";
		static constexpr auto RIGHT = "RIGHT";
		static constexpr auto REPORT = "REPORT";
	}


	class RobotSimulator {
	public:
		explicit RobotSimulator(RobotGrid& world) noexcept;
		virtual ~RobotSimulator();

		void start() noexcept;
		void place(const RobotFactory::RobotLocation& location) noexcept;
		void place(const RobotFactory::RobotLocation& location, const std::string& robot_name) noexcept;
		void report() const noexcept;
		void move() noexcept;
		void rotateLeft() noexcept;
		void rotateRight() noexcept;
		
		// TODO: Support issuing commands to a particular robot
		void move(size_t robot_ID) noexcept;
		void rotateLeft(size_t robot_ID) noexcept;
		void rotateRight(size_t robot_ID) noexcept;
		void report(size_t robot_ID) noexcept;

		// TODO: Modify these to end the robot's meandering
		void end() noexcept;
		void end(size_t robot_ID) noexcept;

		std::shared_ptr<RobotFactory::Robot> getRobot(const RobotFactory::RobotLocation& location) noexcept;
		std::shared_ptr<RobotFactory::Robot> getRobot(const std::string& robot_name) noexcept;
		std::shared_ptr<RobotFactory::Robot> getRobot(const size_t robot_id) noexcept;
	private:
		struct impl;
		std::unique_ptr<impl> m_pImpl;
	};
}