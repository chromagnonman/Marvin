#pragma once

#include <memory>

class RobotWorld;

/**
* @brief 
*/

namespace RobotWorldSimulator {

	struct RobotLocation {

		enum class DIRECTION : size_t {
			SOUTH = 0,
			WEST,
			EAST,
			NORTH
		};

		size_t x_coordinate;
		size_t y_coordinate;
		std::string direction;
	};

	class RobotSimulator {
	public:
		explicit RobotSimulator(RobotWorld& world) noexcept;
		virtual ~RobotSimulator();

		void start() noexcept;
		void place(RobotLocation& location) noexcept;
		void report() const noexcept;
		void move() noexcept;
		void rotateLeft() noexcept;
		void rotateRight() noexcept;
		void end() noexcept;

		[[nodiscard]] const RobotLocation& location() const noexcept;
		[[nodiscard]] std::string currentDirection() const noexcept;

	private:
		struct impl;
		std::unique_ptr<impl> m_pImpl;
	};
}