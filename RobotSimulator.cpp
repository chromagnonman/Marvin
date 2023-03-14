#include <iostream>
#include <vector>
#include <csignal>
#include <sstream>
#include <execution>
#include <algorithm>

#include "RobotSimulator.h"
#include "Marvin.h"
#include "Utils.h"

namespace RobotWorldSimulator {

	volatile std::sig_atomic_t signal_status;

	void signal_handler(int signal)
	{
		signal_status = signal;
	}

	struct HashKey {
		size_t operator()(const std::shared_ptr<RobotFactory::Robot>& robot) const noexcept {
			// We'll just utilize the robot's unique id
			return robot->Id();
		}
	};

	struct Comparator {
		bool operator()(const std::shared_ptr<RobotFactory::Robot>& robot1, const std::shared_ptr<RobotFactory::Robot>& robot2) const noexcept {
			return robot1->Id() != robot2->Id();
		}
	};

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
		std::unordered_set<std::shared_ptr<RobotFactory::Robot>, HashKey, Comparator> m_robots;

		void execute(std::string& command) noexcept;
	};

	RobotSimulator::impl::impl(RobotGrid& grid) noexcept : m_grid{ grid }
	{
		m_robots.reserve(RobotWorldSimulator::DEFAULT_NUMBER_OF_ROBOTS);
	}

	void RobotSimulator::impl::start() noexcept
	{
		std::cout << "Robot excursion started!" << std::endl;

		std::signal(SIGINT, signal_handler);

		std::string command{};
		
		//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	    //std::cin.imbue(std::locale{"en_US.UTF8"});

		Utils::showMenu();

		while (std::getline(std::cin, command)) {

			execute(command);
		}

		std::cout << "\nRobot excursion ended!\n";
	}

	void RobotSimulator::impl::execute(std::string& input) noexcept
	{
		// Replace extraneous characters with spaces
		std::replace_if(std::execution::par_unseq, input.begin(), input.end(), [](const unsigned char c) noexcept {
			return !isalnum(c);
			}, ' ');

		std::string command{};
		std::string direction{};
		size_t x{ 0 };
		size_t y{ 0 };

		{ // Extract the command from the input stream including the parameters
			std::stringstream input_stream{ input };
			input_stream >> command >> x >> y >> direction;
		}

		if (_stricmp(command.c_str(), COMMAND::PLACE) == 0)
		{
			// Check if direction is valid
			place({ x, y, Utils::getDirection(direction) });
		}
		else if (_stricmp(command.c_str(), COMMAND::LEFT) == 0)
		{
			rotateLeft();
		}
		else if (_stricmp(command.c_str(), COMMAND::RIGHT) == 0)
		{
			rotateRight();
		}
		else if (_stricmp(command.c_str(), COMMAND::MOVE) == 0)
		{
			move();
		}
		else if (_stricmp(command.c_str(), COMMAND::REPORT) == 0)
		{
			report();
		}
		else
		{
			std::cerr << "\nInvalid command!\n";
		}
	}

	void RobotSimulator::impl::place(const RobotFactory::RobotLocation& location) noexcept
	{
		std::shared_ptr<RobotFactory::Robot> robot = std::make_shared<RobotFactory::Marvin>(location);

		std::cout << "\nRobot[" << robot->Id() << "] created." << "\nName: " << robot->name()
			<< "\nLocation: (" << location.x_coordinate << "," << location.y_coordinate
			<< ")" << "\ndirection: " << location.direction << '\n';

		m_robots.insert(robot);
		m_grid.addRobot(robot);

		std::cout << "\nNumber of robots: " << m_robots.size() << '\n';
	}

	void RobotSimulator::impl::place(const RobotFactory::RobotLocation& location, const std::string& name) noexcept
	{
		const auto robot = std::make_shared<RobotFactory::Marvin>(location, name);

		std::cout << "\nRobot[" << robot->Id() << "] created." << "\nName: " << robot->name()
			<< "\nLocation: (" << location.x_coordinate << "," << location.y_coordinate
			<< ")" << "\ndirection: " << location.direction << '\n';

		m_robots.insert(robot);
		m_grid.addRobot(robot);

		std::cout << "\nNumber of robots: " << m_robots.size() << '\n';
	}

	void RobotSimulator::impl::report() const noexcept
	{
		std::cout << "Robot info:\n";
		for (const auto& robot : m_robots)
		{
			std::cout << "\nRobot ID: " << robot->Id();
			std::cout << "\nRobot name: " << robot->name();
			std::cout << "\nRobot location (" << robot->location().x_coordinate << "," << robot->location().y_coordinate
				<< ")," << robot->location().direction << '\n';
		}
	}

	void RobotSimulator::impl::move() noexcept
	{
		std::cout << "\nRobot(s) moved\n";
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
		std::cout << "\nRobot turned left\n";
		for (auto& robot : m_robots)
		{
			robot->rotate();
		}
	}

	void RobotSimulator::impl::rotateRight() noexcept
	{
		std::cout << "\nRobot shifted right.\n";
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