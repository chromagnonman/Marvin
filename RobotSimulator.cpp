#include "RobotSimulator.h"
#include "Robot.h"
#include "RobotCommandHandler.h"

#include <iostream>

namespace RobotWorldSimulator {

	struct RobotSimulator::impl {

		impl(RobotWorld& world) noexcept;

		void start() noexcept;
		void place(const RobotLocation& location) noexcept;
		void report() const noexcept;
		void move() noexcept;
		void rotateLeft() noexcept;
		void rotateRight() noexcept;
		void end() noexcept;

	private:
		RobotWorld& m_world;
	};

	RobotSimulator::impl::impl(RobotWorld& world) noexcept : m_world{ world }
	{
	}

	void RobotSimulator::impl::start() noexcept
	{
		std::cout << "Robot excursion started!\n";
	}

	void RobotSimulator::impl::place(const RobotLocation& location) noexcept
	{
		std::cout << "Robot created in location: " << location.x_coordinate << "," << location.y_coordinate << " direction: " << location.direction << '\n';
	}

	void RobotSimulator::impl::report() const noexcept
	{
		std::cout << "Robot location:";
	}

	void RobotSimulator::impl::move() noexcept
	{
		std::cout << "Robot moved!\n";
	}

	void RobotSimulator::impl::rotateLeft() noexcept
	{
		std::cout << "Robot rotated to the left side!\n";
	}

	void RobotSimulator::impl::rotateRight() noexcept
	{
		std::cout << "Robot rotated to the right side!\n";
	}

	void RobotSimulator::impl::end() noexcept
	{
		std::cout << "Robot excursion ended!\n";
	}


}