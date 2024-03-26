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


    using namespace RobotFactory::ROBOT_DIRECTION;
    using namespace RobotWorldSimulator::COMMAND;

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
        void end(size_t robotId) noexcept;
        const std::unique_ptr<RobotFactory::Robot>& getRobot(const RobotFactory::RobotLocation& location) const noexcept;
        const std::unique_ptr<RobotFactory::Robot>& getRobot(size_t robot_id) const noexcept;

    private:
        RobotGrid& m_grid;
        std::unordered_map<size_t, std::unique_ptr<RobotFactory::Robot>> m_robots;

        std::string getDirection(const std::string& direction) const noexcept;
        bool isGridEmpty() const noexcept;
        void showUsage() const noexcept;
        void showDetails(const std::unique_ptr<RobotFactory::Robot>& robot) const noexcept;
        void execute(std::string& command) noexcept;

    };

    RobotSimulator::impl::impl(RobotGrid& grid) noexcept : m_grid{grid}
    {
    }

    std::string RobotSimulator::impl::getDirection(const std::string& direction) const noexcept
    {
        if (direction == NORTH)
        {
            return NORTH;
        }
        else if (direction == SOUTH)
        {
            return SOUTH;
        }
        else if (direction == EAST)
        {
            return EAST;
        }
        else if (direction == WEST)
        {
            return WEST;
        }

        return NORTH;
    }

    bool RobotSimulator::impl::isGridEmpty() const noexcept
    {
        if (m_robots.empty())
        {
            std::cout << "\nThere are no robots in the grid yet. Use Place command.\n";
            return true;
        }

        return false;
    }

    void RobotSimulator::impl::showUsage() const noexcept
    {
        std::cout << "\nUsage: PLACE X,Y, Direction (NORTH, SOUTH, EAST, WEST) Name (optional) i.e. PLACE 2,2, NORTH, R2D2";
        std::cout << "\n       MOVE";
        std::cout << "\n       LEFT";
        std::cout << "\n       RIGHT";
        std::cout << "\n       REPORT";
        std::cout << "\nCtrl+C to quit";
        std::cout << "\n\n> ";
    }

    void RobotSimulator::impl::showDetails(const std::unique_ptr<RobotFactory::Robot>& robot) const noexcept
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
        int32_t x{0};
        int32_t y{0};

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

        std::transform(std::execution::par, command.begin(), command.end(), command.begin(),
            [] (char c) { 
                return std::toupper(c);
            });

        std::transform(std::execution::par, direction.begin(), direction.end(), direction.begin(),
            [](char c) { 
                return std::toupper(c); 
            });

        if (command == PLACE)
        {
            place({ x, y, getDirection(direction) }, name);
        }
        else if (command == LEFT)
        {
            rotateLeft();
        }
        else if (command == RIGHT)
        {
            rotateRight();
        }
        else if (command == MOVE)
        {
            move();
        }
        else if (command == REPORT)
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
        std::unique_ptr<RobotFactory::Robot> robot = std::make_unique<RobotFactory::Marvin>(location, name);

        showDetails(robot);

        if (!m_grid.isOffTheGrid(robot)) 
        {
            m_grid.addRobot(robot->Id(), robot->location());
        }
        else 
        {
            // Reset location
            robot->setLocation({0, 0, robot->location().direction});
        }

        m_robots.emplace(robot->Id(), std::move(robot));

        std::cout << "\nNumber of robots in the grid: " << m_robots.size() << '\n';
    }

    void RobotSimulator::impl::report() const noexcept
    {
        if (!isGridEmpty())
        {
            for (const auto& [_, robot] : m_robots)
            {
                showDetails(robot);
            }
        }
    }

    void RobotSimulator::impl::move() noexcept
    {
        if (!isGridEmpty()) 
        {
            for (auto& [_, robot] : m_robots)
            {
                const auto current_location = robot->location();
                robot->move();

                if (!m_grid.isOffTheGrid(robot)) // TODO: Also check if location is already occupied by another robot.
                {
                    std::cout << '\n' << robot->name() << " moved one unit forward heading " << robot->location().direction << '\n';
                    m_grid.updateLocation(current_location, robot->location(), robot->Id());
                }
                else
                {
                    std::cout << '\n' << robot->name() << " is already at the edge of the grid, facing " << robot->location().direction << '\n';
                    // Revert to previous location
                    robot->setLocation(current_location);
                }
            }
        }
    }

    void RobotSimulator::impl::rotateLeft() noexcept
    {
        if (!isGridEmpty())
        {
            for (auto& [_, robot] : m_robots)
            {
                robot->rotate();
                std::cout << '\n' << robot->name() << " turned left facing " << robot->location().direction << '\n';
            }
        }
    }

    void RobotSimulator::impl::rotateRight() noexcept
    {
        if (!isGridEmpty())
        {
            for (auto& [_, robot] : m_robots)
            {
                robot->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);
                std::cout << '\n' << robot->name() << " shifted right facing " << robot->location().direction << '\n';
            }
        }
    }

    const std::unique_ptr<RobotFactory::Robot>& RobotSimulator::impl::getRobot(const RobotFactory::RobotLocation& location) const noexcept 
    {
      return getRobot(m_grid.getRobotID(location));
    }

    const std::unique_ptr<RobotFactory::Robot>& RobotSimulator::impl::getRobot(size_t robot_id) const noexcept 
    {
      if (const auto found_robot = m_robots.find(robot_id); found_robot != m_robots.end()) 
      {
        return found_robot->second;
      }

      return nullptr;
    }

    void RobotSimulator::impl::end(size_t robotId) noexcept
    {
        std::cout << "\nRobot excursion ended!\n";
        m_robots.erase(robotId);
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

    const std::unique_ptr<RobotFactory::Robot>& RobotSimulator::getRobot(const RobotFactory::RobotLocation& location) const noexcept
    {
        return m_pImpl->getRobot(location);
    }

    const std::unique_ptr<RobotFactory::Robot>& RobotSimulator::getRobot(size_t robot_id) const noexcept
    {
        return m_pImpl->getRobot(robot_id);
    }

    void RobotSimulator::end(size_t robotId) noexcept
    {
        m_pImpl->end(robotId);
    }
}