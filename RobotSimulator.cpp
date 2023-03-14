#include "RobotSimulator.h"
#include "Robot.h"
#include "Marvin.h"
//#include "RobotCommandHandler.h"

#include <iostream>
#include <vector>
#include <csignal>
#include <algorithm>
#include <sstream>

// TODO: Use unordered_set to look up a robot using its ID.
//namespace {
//
//	struct HashKey {
//		size_t operator()(const RobotFactory::Marvin& robot) const noexcept {
//			// We'll just use the robot's unique id
//			return robot.Id();
//		}
//	};
//
//	struct Comparator {
//		bool operator()(const RobotFactory::Marvin& robot1, const RobotFactory::Marvin& robot2) const noexcept {
//			return robot1.Id() != robot2.Id();
//		}
//	};
//}

namespace {
	volatile std::sig_atomic_t signal_status;
}

void signal_handler(int signal)
{
	signal_status = signal;
}

namespace RobotWorldSimulator {

	struct RobotSimulator::impl {

		impl(RobotGrid& world) noexcept;

		void start() noexcept;
		void place(const RobotFactory::RobotLocation& location) noexcept;
		void place(const RobotFactory::RobotLocation& location, const std::string& name) noexcept;
		void report() const noexcept;
		void move() noexcept;
		void rotateLeft() noexcept;
		void rotateRight() noexcept;
		void end() noexcept;

	private:
		RobotGrid& m_grid;
		// TODO: Use set to look up a robot using its ID.
		//std::unordered_set<RobotFactory::Marvin, HashKey, Comparator> m_robots;
		std::vector<std::shared_ptr<RobotFactory::Robot>> m_robots;
	};

	RobotSimulator::impl::impl(RobotGrid& grid) noexcept : m_grid{ grid }
	{
	}

	void RobotSimulator::impl::start() noexcept
	{
		std::cout << "Robot excursion started!" << std::endl;

		std::signal(SIGINT, signal_handler);

		std::cout << "Usage: PLACE X,Y, NORTH\n";
		std::string input{};
		std::stringstream ss;

		while (std::getline(std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'), input)) {
			//std::cin.uppercase()
			std::replace(input.begin(), input.end(), ',', ' ');
			ss << input;
			std::cout << input<< std::endl;
			input = "";
		}

		end();
	}

	void RobotSimulator::impl::place(const RobotFactory::RobotLocation& location) noexcept
	{
		const auto robot = std::make_shared<RobotFactory::Marvin>(location);

		std::cout << "\nRobot[" << robot->Id() << "] created." << "\nLocation(" << location.x_coordinate << "," 
			<< location.y_coordinate << ")" << "direction: " << location.direction;

		m_robots.push_back(robot);
		m_grid.addRobot(robot);
		std::cout << "Number of robots: " << m_robots.size();
	}

	void RobotSimulator::impl::place(const RobotFactory::RobotLocation& location, const std::string& name) noexcept
	{
		const auto robot = std::make_shared<RobotFactory::Marvin>(location, name);

		std::cout << "\nRobot[" << robot->Id() << "] created." << "\nLocation(" << location.x_coordinate << ","
			<< location.y_coordinate << ")" << "direction: " << location.direction;

		m_robots.push_back(robot);
		m_grid.addRobot(robot);
		std::cout << "Number of robots: " << m_robots.size();
	}

	void RobotSimulator::impl::report() const noexcept
	{
		std::cout << "Robot info:\n";
		for (const auto& robot : m_robots)
		{
			std::cout << "\nRobot ID: " << robot->Id();
			std::cout << "\nRobot name: " << (robot->name().empty()) ? "unanmed" : robot->name();
			std::cout << "\nRobot location (" << robot->location().x_coordinate << "," << robot->location().y_coordinate
				<< ")," << robot->location().direction;
		}
	}

	void RobotSimulator::impl::move() noexcept
	{
		std::cout << "\nMoving robot(s)";
		for (auto& robot : m_robots)
		{
			const auto current_location = robot->location();
			robot->move();

			if (!m_grid.isOffTheGrid(robot->location())) // TODO: Also check if location is already occupied by another robot.
			{
				m_grid.updateLocation(current_location, robot);
			}
			else
			{
				// Revert to previous location
				robot->setLocation(current_location);
			}
		}
	}

	void RobotSimulator::impl::rotateLeft() noexcept
	{
		std::cout << "\nRotating left...";
		for (auto& robot : m_robots)
		{
			robot->rotate();
		}
	}

	void RobotSimulator::impl::rotateRight() noexcept
	{
		std::cout << "\nRotating right...";
		for (auto& robot : m_robots)
		{
			robot->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);
		}
	}

	void RobotSimulator::impl::end() noexcept
	{
		std::cout << "\nRobot excursion ended!\n";
		m_robots.clear();
	}

	RobotSimulator::RobotSimulator(RobotGrid& world) noexcept : m_pImpl{ std::make_unique<impl>(world) }
	{
	}

	RobotSimulator::~RobotSimulator() = default;

	void RobotSimulator::start() noexcept
	{
		m_pImpl->start();
	}

	void RobotSimulator::place(const RobotFactory::RobotLocation& location) noexcept
	{
		m_pImpl->place(location);
	}

	void RobotSimulator::place(const RobotFactory::RobotLocation& location, const std::string& robot_name) noexcept
	{
		m_pImpl->place(location, robot_name);
	}

	void RobotSimulator::report() const noexcept
	{
		m_pImpl->report();
	}

	void RobotSimulator::move() noexcept
	{
		m_pImpl->move();
	}

	void RobotSimulator::rotateLeft() noexcept
	{
		m_pImpl->rotateLeft();
	}

	void RobotSimulator::rotateRight() noexcept
	{
		m_pImpl->rotateRight();
	}

	void RobotSimulator::end() noexcept
	{
		m_pImpl->end();
	}
}