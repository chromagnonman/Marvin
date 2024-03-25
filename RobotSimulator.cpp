#include <iostream>
#include <vector>
#include <csignal>
#include <sstream>
#include <execution>
#include <algorithm>
#include <unordered_map>

#include "RobotSimulator.h"
#include "Marvin.h"

namespace RobotWorldSimulator {

	volatile std::sig_atomic_t signal_status;

	void signal_handler(int signal)
	{
		signal_status = signal;
	}

	struct RobotSimulator::impl {

		impl(RobotGrid& world) noexcept;

		void start() noexcept;
		void place(const RobotFactory::RobotLocation& location, const std::string& name) noexcept;
		void report() const noexcept;
		void move() noexcept;
		void rotateLeft() noexcept;
		void rotateRight() noexcept;
		void end() noexcept;
		std::shared_ptr<RobotFactory::Robot> getRobot(const RobotFactory::RobotLocation& location) noexcept;
		std::shared_ptr<RobotFactory::Robot> getRobot(const size_t robot_id) noexcept;
		
	private:
		RobotGrid& m_grid;
		std::unordered_map<size_t, std::shared_ptr<RobotFactory::Robot>> m_robots;

		std::string getDirection(const std::string& direction) const noexcept;
		void checkGrid() const noexcept;
		void showUsage() const noexcept;
		void showReport(const std::shared_ptr<RobotFactory::Robot>& robot) const noexcept;
		void execute(std::string& command) noexcept;
	};

	RobotSimulator::impl::impl(RobotGrid& grid) noexcept : m_grid{ grid }
	{
		m_robots.reserve(RobotWorldSimulator::DEFAULT_NUMBER_OF_ROBOTS);
	}

	std::string RobotSimulator::impl::getDirection(const std::string& direction) const noexcept
	{
		using namespace RobotFactory::ROBOT_DIRECTION;
		if (_stricmp(direction.c_str(), NORTH) == 0)
		{
			return NORTH;
		}
		else if (_stricmp(direction.c_str(), SOUTH) == 0)
		{
			return SOUTH;
		}
		else if (_stricmp(direction.c_str(), EAST) == 0)
		{
			return EAST;
		}
		else if (_stricmp(direction.c_str(), WEST) == 0)
		{
			return WEST;
		}

		return NORTH;
	}

	void RobotSimulator::impl::checkGrid() const noexcept
	{
		if (m_robots.empty())
		{
			std::cout << "\nThere are no robots in the grid yet. Use Place command.\n";
			return;
		}
	}

	void RobotSimulator::impl::showUsage() const noexcept
	{
		std::cout << "\nConstraints: X and Y should not be greater than or equal to ten.";
		std::cout << "\nUsage: PLACE X,Y, Direction (NORTH, SOUTH, EAST, WEST) Name (optional) i.e. PLACE 2,2, NORTH, R2D2";
		std::cout << "\n       MOVE";
		std::cout << "\n       LEFT";
		std::cout << "\n       RIGHT";
		std::cout << "\n       REPORT";
		std::cout << "\n\n> ";
	}

	void RobotSimulator::impl::showReport(const std::shared_ptr<RobotFactory::Robot>& robot) const noexcept
	{
		std::cout << "\nRobot ID: " << robot->Id();
		std::cout << "\nRobot name: " << robot->name();
		std::cout << "\nRobot location (" << robot->location().x_coordinate << "," << robot->location().y_coordinate
			<< ")," << robot->location().direction << '\n';
	}

	void RobotSimulator::impl::start() noexcept
	{
		std::signal(SIGINT, signal_handler);

		std::string command{};
		
		showUsage();

		while (std::getline(std::cin, command)) {

			execute(command);
		}
	}

	void RobotSimulator::impl::execute(std::string& input) noexcept
	{
		// Replace extraneous characters with spaces
		std::replace_if(std::execution::par_unseq, input.begin(), input.end(), [](const unsigned char c) noexcept {
			return !isalnum(c);
			}, ' ');

		std::string command{};
		std::string name{"Marvin"};
		std::string direction{};
		int x{ 0 };
		int y{ 0 };

		{ // Extract the command from the input stream including the parameters
			std::stringstream input_stream{ input };
			input_stream >> command >> x >> y >> direction >> name;
		}

		// If XY is greater than or equal to the default size, reset the location to zero. 
		if (x >= m_grid.getSize().width || y >= m_grid.getSize().height)
		{
			// TODO: Resize the grid
			x = 0;
			y = 0;
		}

		if (_stricmp(command.c_str(), COMMAND::PLACE) == 0)
		{
			place({ x, y, getDirection(direction) }, name);
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

	void RobotSimulator::impl::place(const RobotFactory::RobotLocation& location, const std::string& name) noexcept
	{
		std::shared_ptr<RobotFactory::Robot> robot = std::make_shared<RobotFactory::Marvin>(location, name);

		showReport(robot);

		m_robots.emplace(std::make_pair(robot->Id(), robot));
		m_grid.addRobot(robot);

		std::cout << "\nNumber of robots in the grid: " << m_robots.size() << '\n';
	}

	void RobotSimulator::impl::report() const noexcept
	{
		checkGrid();

		for (const auto& [_, robot] : m_robots)
		{
			showReport(robot);
		}
	}

	void RobotSimulator::impl::move() noexcept
	{
		checkGrid();

		for (auto& [_, robot] : m_robots)
		{
			const auto current_location = robot->location();
			robot->move();

			if (!m_grid.isOffTheGrid(robot)) // TODO: Also check if location is already occupied by another robot.
			{
				std::cout << '\n' << robot->name() << " moved one unit forward heading " << robot->location().direction << '\n';
				m_grid.updateLocation(current_location, robot);
			}
			else
			{
				std::cout << '\n' << robot->name() << " is already at the edge of the grid, facing " << robot->location().direction << '\n';
				// Revert to previous location
				robot->setLocation(current_location);
			}
		}
	}

	void RobotSimulator::impl::rotateLeft() noexcept
	{
		checkGrid();

		for (auto& [_, robot] : m_robots)
		{
			robot->rotate();
			std::cout << '\n' << robot->name() << " turned left facing " << robot->location().direction << '\n';
		}
	}

	void RobotSimulator::impl::rotateRight() noexcept
	{
		checkGrid();

		for (auto& [_, robot] : m_robots)
		{
			robot->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);
			std::cout << '\n' << robot->name() << " shifted right facing " << robot->location().direction << '\n';
		}
	}

	void RobotSimulator::impl::end() noexcept
	{
		std::cout << "\nRobot excursion ended!\n";
		m_robots.clear();
	}

	std::shared_ptr<RobotFactory::Robot> RobotSimulator::impl::getRobot(const RobotFactory::RobotLocation& location) noexcept
	{
		return m_grid.getRobot(location);
	}

	std::shared_ptr<RobotFactory::Robot> RobotSimulator::impl::getRobot(const size_t robot_id) noexcept
	{
		if (const auto found_robot = m_robots.find(robot_id); found_robot != m_robots.end())
		{
			return found_robot->second;
		}

		return nullptr;
	}

	RobotSimulator::RobotSimulator(RobotGrid& world) noexcept : m_pImpl{ std::make_unique<impl>(world) }
	{
		m_pImpl->start();
	}

	RobotSimulator::~RobotSimulator() = default;

	
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

	std::shared_ptr<RobotFactory::Robot> RobotSimulator::getRobot(const RobotFactory::RobotLocation& location) noexcept
	{
		return m_pImpl->getRobot(location);
	}

	std::shared_ptr<RobotFactory::Robot> RobotSimulator::getRobot(const size_t robot_id) noexcept
	{
		return m_pImpl->getRobot(robot_id);
	}
}