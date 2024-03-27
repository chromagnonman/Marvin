#include <iostream>
#include <vector>
#include <csignal>
#include <sstream>
#include <execution>
#include <algorithm>
#include <unordered_map>

#include "RobotSimulator.h"
#include "Marvin.h"
#include "Menu.h"

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
        
        // Affects all robots in the grid
        void report() const noexcept;
        void move() noexcept;
        void rotateLeft() noexcept;
        void rotateRight() noexcept;
        void removeAll() noexcept;

        // Individual robot commands
        void move(size_t robot_ID) noexcept;
        void rotateLeft(size_t robot_ID) noexcept;
        void rotateRight(size_t robot_ID) noexcept;
        void remove(size_t robot_ID) noexcept;

        const std::unique_ptr<RobotFactory::Robot>& getRobot(const RobotFactory::RobotLocation& location) const noexcept;
        const std::unique_ptr<RobotFactory::Robot>& getRobot(size_t robot_id) const noexcept;

    private:
        RobotGrid& m_grid;
        std::unordered_map<size_t, std::unique_ptr<RobotFactory::Robot>> m_robots;

        std::string getDirection(const std::string& direction) const noexcept;
        bool isGridEmpty() const noexcept;
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
            std::cout << "\nThere are no robots in the grid.\n";
            Menu::showUsage();
            return true;
        }

        return false;
    }

    void RobotSimulator::impl::start() noexcept
    {
        std::signal(SIGINT, signal_handler);

        std::string command;
        
        Menu::showUsage();

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

        std::string command;
        std::string name{"Marvin"};
        std::string direction;
        int32_t x{0};
        int32_t y{0};

        { // Extract the command from the input stream including the parameters
            std::istringstream input_stream{ input };
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
            size_t robot_ID{0};
            {
                std::stringstream input_stream{input};
                input_stream >> command >> robot_ID;
            }

            if (robot_ID) 
            {
                rotateLeft(robot_ID);
            }
            else 
            {
                rotateLeft();
            }
        }
        else if (command == RIGHT)
        {
            size_t robot_ID{0};
            {
                std::stringstream input_stream{input};
                input_stream >> command >> robot_ID;
            }

            if (robot_ID) 
            {
                rotateRight(robot_ID);
            } 
            else 
            {
                rotateRight();
            }
        }
        else if (command == MOVE)
        {
            size_t robot_ID{0};
            {
                std::stringstream input_stream{input};
                input_stream >> command >> robot_ID;
            }

            if (robot_ID) 
            {
              move(robot_ID);
            } 
            else 
            {
              move();
            }
        }
        else if (command == REMOVE) 
        {
           size_t robot_ID{0};
           {
               std::stringstream input_stream{input};
               input_stream >> command >> robot_ID;
           }

           if (robot_ID) 
           {
               remove(robot_ID);
           } 
           else 
           {
               removeAll();
           }
        } 
        else if (command == REPORT) 
        {
            report();
        }
        else if (command == MENU) 
        {
          Menu::showUsage();
        }
        else
        {
            std::cerr << "\nInvalid command!\n";
            Menu::showUsage();
        }
    }

    void RobotSimulator::impl::place(const RobotFactory::RobotLocation& location, const std::string& name) noexcept
    {
        std::unique_ptr<RobotFactory::Robot> robot = std::make_unique<RobotFactory::Marvin>(location, name);

        if (m_grid.isOffTheGrid(robot) || m_grid.isOccupied(robot)) 
        {
            // TODO: look for an empty slot in the grid
            // Reset location
            std::cout << "\nLocation is occupied/off the grid.\n";
            robot->setLocation({0, 0, robot->location().direction});
        }

        Menu::showDetails(robot);

        m_grid.addRobot(robot->Id(), robot->location());
        m_robots.emplace(robot->Id(), std::move(robot));

        std::cout << "\nNumber of robots in the grid: " << m_robots.size() << '\n';
    }

    void RobotSimulator::impl::report() const noexcept
    {
        if (!isGridEmpty())
        {
            std::cout << "\nFound " << m_robots.size() << " robots in the grid\n";
            for (const auto& [_, robot] : m_robots)
            {
                Menu::showDetails(robot);
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

                if (!m_grid.isOffTheGrid(robot))
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

    void RobotSimulator::impl::move(size_t robot_ID) noexcept 
    {
        if (!isGridEmpty()) 
        {
            if (auto robot = m_robots.find(robot_ID); robot != m_robots.end())
            {
                const auto current_location = robot->second->location();
                robot->second->move();

                if (!m_grid.isOffTheGrid(robot->second) && !m_grid.isOccupied(robot->second))
                {
                    std::cout << '\n' << robot->second->name() << " moved one unit forward heading " << robot->second->location().direction << '\n';
                    m_grid.updateLocation(current_location, robot->second->location(), robot->second->Id());
                } 
                else 
                {
                    std::cout << '\n' << robot->second->name() << " is unable to move. Slot is occupied or outside the grid.\n";
                    // Revert to previous location
                    robot->second->setLocation(current_location);
                 }
            }
            else {
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

    void RobotSimulator::impl::rotateLeft(size_t robot_ID) noexcept 
    {
        if (!isGridEmpty()) 
        {
            if (auto robot = m_robots.find(robot_ID); robot != m_robots.end())
            {
                robot->second->rotate();
                std::cout << '\n' << robot->second->name() << " turned left facing " << robot->second->location().direction << '\n';
            }
        }
        else 
        {
            std::cout << "Robot ID: " << robot_ID << " not found!\n";
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

    void RobotSimulator::impl::rotateRight(size_t robot_ID) noexcept 
    {
        if (!isGridEmpty()) 
        {
            if (auto robot = m_robots.find(robot_ID); robot != m_robots.end()) 
            {
                robot->second->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);
                std::cout << '\n' << robot->second->name() << " turned right facing " << robot->second->location().direction << '\n';
            }
            else 
            {
                std::cout << "Robot ID: " << robot_ID << " not found!\n";
            }
        }
    }

    void RobotSimulator::impl::removeAll() noexcept 
    { 
        if (!isGridEmpty()) 
        {
            for (const auto& robot : m_robots) 
            {
               m_grid.remove(robot.second);
            }
            m_robots.clear();
            std::cout << "\nAll robots were terminated!\n";
            Menu::showUsage();
        }
    }

    void RobotSimulator::impl::remove(size_t robot_ID) noexcept 
    { 
        if (auto robot = m_robots.find(robot_ID); robot != m_robots.end()) 
        {
            m_grid.remove(robot->second);
            std::cout << "\nThe following robot was removed.\n";
            Menu::showDetails(robot->second);

            m_robots.erase(robot_ID);
        }
        else 
        {
            std::cout << "Robot ID: " << robot_ID << " not found!\n";
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

    void RobotSimulator::move(size_t robot_ID) noexcept 
    { 
        m_pImpl->move(robot_ID); 
    }

    void RobotSimulator::rotateLeft() noexcept
    {
        m_pImpl->rotateLeft();
    }

    void RobotSimulator::rotateLeft(size_t robot_ID) noexcept 
    {
        m_pImpl->rotateLeft(robot_ID); 
    }

    void RobotSimulator::rotateRight() noexcept
    {
        m_pImpl->rotateRight();
    }

    void RobotSimulator::rotateRight(size_t robot_ID) noexcept
    { 
        m_pImpl->rotateRight(robot_ID);
    }

    void RobotSimulator::removeAll() noexcept 
    { 
        m_pImpl->removeAll();
    }

    const std::unique_ptr<RobotFactory::Robot>& RobotSimulator::getRobot(const RobotFactory::RobotLocation& location) const noexcept
    {
        return m_pImpl->getRobot(location);
    }

    const std::unique_ptr<RobotFactory::Robot>& RobotSimulator::getRobot(size_t robot_id) const noexcept
    {
        return m_pImpl->getRobot(robot_id);
    }

    void RobotSimulator::remove(size_t robot_ID) noexcept
    {
        m_pImpl->remove(robot_ID);
    }
}