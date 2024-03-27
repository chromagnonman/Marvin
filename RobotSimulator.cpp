#include <iostream>
#include <vector>
#include <csignal>
#include <unordered_map>

#include "RobotSimulator.h"
#include "Marvin.h"
#include "Menu.h"
#include "Utils.h"

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
        void place(const RobotParameters&) noexcept;
        
        // Affects all robots in the grid
        void report() const noexcept;
        void move() noexcept;
        void rotateLeft() noexcept;
        void rotateRight() noexcept;
        void removeAll() noexcept;

        // Individual robot commands
        void move(const RobotParameters&) noexcept;
        void rotateLeft(const RobotParameters&) noexcept;
        void rotateRight(const RobotParameters&) noexcept;
        void remove(const RobotParameters&) noexcept;

        const std::unique_ptr<RobotFactory::Robot>& getRobot(const RobotFactory::RobotLocation& location) const noexcept;
        const std::unique_ptr<RobotFactory::Robot>& getRobot(size_t robot_id) const noexcept;

    private:
        RobotGrid& m_grid;
        std::unordered_map<size_t, std::unique_ptr<RobotFactory::Robot>> m_robots;

        bool isGridEmpty() const noexcept;
        void execute(std::string& command) noexcept;

    };

    RobotSimulator::impl::impl(RobotGrid& grid) noexcept : m_grid{grid}
    {
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

    void RobotSimulator::impl::execute(std::string& command) noexcept
    {
        // TODO: Move all these input processing to a utility class
        RobotParameters robot;

        Utils::processInputParams(robot, command);

        if (robot.command == PLACE)
        {
            //  If XY is greater than or equal to the default size, reset the
            //  location to zero.
            if (robot.location.x_coordinate >= m_grid.getSize().width || 
                robot.location.y_coordinate >= m_grid.getSize().height) 
            {
                // TODO: Resize the grid
                robot.location.x_coordinate = 0;
                robot.location.y_coordinate = 0;
            }

            place(robot);
        }
        else if (robot.command == LEFT)
        {
            Utils::processCommandParams(robot, command);

            if (robot.ID) 
            {
                rotateLeft(robot);
            }
            else 
            {
                rotateLeft();
            }
        }
        else if (robot.command == RIGHT)
        {
            Utils::processCommandParams(robot, command);
            
            if (robot.ID) 
            {
                rotateRight(robot);
            } 
            else 
            {
                rotateRight();
            }
        }
        else if (robot.command == MOVE)
        {
            Utils::processCommandParams(robot, command);

            if (robot.ID) 
            {
              move(robot);
            } 
            else 
            {
              move();
            }
        }
        else if (robot.command == REMOVE) 
        {
            Utils::processCommandParams(robot, command);

            if (robot.ID) 
            {
                remove(robot);
            } 
            else 
            {
                removeAll();
            }
        } 
        else if (robot.command == REPORT) 
        {
            report();
        }
        else if (robot.command == MENU) 
        {
            Menu::showUsage();
        }
        else
        {
            std::cerr << "\nInvalid command!\n";
            Menu::showUsage();
        }
    }

    void RobotSimulator::impl::place(const RobotParameters& robot) noexcept
    {
        std::unique_ptr<RobotFactory::Robot> new_robot = std::make_unique<RobotFactory::Marvin>(robot.location, robot.name);

        if (m_grid.isOffTheGrid(new_robot) || m_grid.isOccupied(new_robot)) 
        {
            // TODO: look for an empty slot in the grid
            // Reset location
            std::cout << "\nLocation is occupied/off the grid.\n";
            new_robot->setLocation({0, 0, new_robot->location().direction});
        }

        Menu::showDetails(new_robot);

        m_grid.addRobot(new_robot->Id(), new_robot->location());
        m_robots.emplace(new_robot->Id(), std::move(new_robot));

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

    void RobotSimulator::impl::move(const RobotParameters& robot) noexcept 
    {
        if (!isGridEmpty()) 
        {
        if (auto search = m_robots.find(robot.ID); search != m_robots.end())
            {
                const auto current_location = search->second->location();
                // Attempt to move robot
                search->second->move();

                // Checks if location is outside the grid or is occupied by another robot
                if (!m_grid.isOffTheGrid(search->second) && !m_grid.isOccupied(search->second))
                {
                    std::cout << '\n' << search->second->name() << " moved one unit forward heading " 
                              << search->second->location().direction << '\n';
                    
                    m_grid.updateLocation(current_location, search->second->location(), search->second->Id());
                } 
                else 
                {
                    std::cout << '\n' << search->second->name() << " is unable to move. Area is occupied or outside the grid.\n";
                    
                    // Revert to previous location
                    search->second->setLocation(current_location);
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

    void RobotSimulator::impl::rotateLeft(const RobotParameters& robot) noexcept 
    {
        if (!isGridEmpty()) 
        {
            if (auto search = m_robots.find(robot.ID); search != m_robots.end())
            {
                search->second->rotate();

                std::cout << '\n' << search->second->name() << " turned left facing " 
                          << search->second->location().direction << '\n';
            }
        }
        else 
        {
            std::cout << "Robot ID: " << robot.ID << " not found!\n";
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

    void RobotSimulator::impl::rotateRight(const RobotParameters& robot) noexcept 
    {
        if (!isGridEmpty()) 
        {
            if (auto search = m_robots.find(robot.ID); search != m_robots.end()) 
            {
                search->second->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);
                std::cout << '\n' << search->second->name() << " turned right facing " << search->second->location().direction << '\n';
            }
            else 
            {
                std::cout << "Robot ID: " << robot.ID << " not found!\n";
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

    void RobotSimulator::impl::remove(const RobotParameters& robot) noexcept 
    { 
        if (auto search = m_robots.find(robot.ID); search != m_robots.end()) 
        {
            m_grid.remove(search->second);

            std::cout << "\nThe following robot was removed.\n";
            Menu::showDetails(search->second);

            m_robots.erase(robot.ID);
        }
        else 
        {
            std::cout << "Robot ID: " << robot.ID << " not found!\n";
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

    
    void RobotSimulator::place(const RobotParameters& robot) noexcept
    {
        m_pImpl->place(robot);
    }

    void RobotSimulator::report() const noexcept
    {
        m_pImpl->report();
    }

    void RobotSimulator::move() noexcept
    {
        m_pImpl->move();
    }

    void RobotSimulator::move(const RobotParameters& robot) noexcept 
    { 
        m_pImpl->move(robot); 
    }

    void RobotSimulator::rotateLeft() noexcept
    {
        m_pImpl->rotateLeft();
    }

    void RobotSimulator::rotateLeft(const RobotParameters& robot) noexcept 
    {
        m_pImpl->rotateLeft(robot); 
    }

    void RobotSimulator::rotateRight() noexcept
    {
        m_pImpl->rotateRight();
    }

    void RobotSimulator::rotateRight(const RobotParameters& robot) noexcept
    { 
        m_pImpl->rotateRight(robot);
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

    void RobotSimulator::remove(const RobotParameters& robot) noexcept
    {
        m_pImpl->remove(robot);
    }
}