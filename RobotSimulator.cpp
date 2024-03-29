#include <iostream>
#include <vector>
#include <csignal>
#include <unordered_map>

#include "RobotSimulator.h"
#include "Marvin.h"
#include "Menu.h"
#include "Utils.h"

namespace Simulator {

    using namespace RobotFactory::ROBOT_DIRECTION;
    using namespace Simulator::COMMAND;

    volatile std::sig_atomic_t signal_status;

    void signal_handler(int signal)
    {
        signal_status = signal;
    }

    struct RobotSimulator::impl {

        impl(RobotGrid& world) noexcept;

        void start() noexcept;
        bool place(const RobotParameters&) noexcept;
        
        // Affects all robots in the grid
        void move() noexcept;
        void rotateLeft() noexcept;
        void rotateRight() noexcept;
        void removeAll() noexcept;
        void report() const noexcept;


        // Individual robot commands
        bool move(const RobotParameters&) noexcept;
        bool rotateLeft(const RobotParameters&) noexcept;
        bool rotateRight(const RobotParameters&) noexcept;
        bool remove(const RobotParameters&) noexcept;

        // Private
        RobotGrid& m_grid;
        std::unordered_map<std::string, std::unique_ptr<RobotFactory::Robot>> m_robots;

        bool isGridEmpty() const noexcept;
        void execute(std::string&& command) noexcept;

    };

    RobotSimulator::impl::impl(RobotGrid& grid) noexcept : m_grid{grid}
    {
    }

    bool RobotSimulator::impl::isGridEmpty() const noexcept
    {
        if (m_robots.empty())
        {
            std::cout << "\nGrid is empty!\n\n";
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

            execute(std::move(command));
        }
    }

    void RobotSimulator::impl::execute(std::string&& command) noexcept
    {
        RobotParameters robot;

        Utils::processInputParams(robot, command);

        if (robot.command == PLACE)
        {
            //  If XY is greater than or equal to the default size, reset the
            //  location to zero.
            if (robot.location.x_coordinate >= m_grid.getSize().width || 
                robot.location.y_coordinate >= m_grid.getSize().height) 
            {
                robot.location.x_coordinate = 0;
                robot.location.y_coordinate = 0;
            }

            // Handle user error on not specifying a name
            if (robot.name.empty()) 
            {
                robot.name = "R2D2";
            }

            place(robot);
        }
        else if (robot.command == LEFT)
        {
            Utils::processCommandParams(robot, command);

            if (!robot.name.empty()) 
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
            
            if (!robot.name.empty()) 
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

            if (!robot.name.empty()) 
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

            if (!robot.name.empty()) 
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

    bool RobotSimulator::impl::place(const RobotParameters& robot) noexcept
    {
        std::unique_ptr<RobotFactory::Robot> new_robot = std::make_unique<RobotFactory::Marvin>(robot.location, robot.name);

        if (m_grid.isOffTheGrid(new_robot) || m_grid.isOccupied(new_robot)) 
        {
            // TODO: look for an empty slot in the grid if (0,0) is also occupied
            // Reset location
            std::cout << "\nLocation is occupied/outside the grid!\n";
            new_robot->setLocation({0, 0, new_robot->location().direction});
        }

        Menu::showDetails(new_robot);

        m_grid.addRobot(new_robot->Id(), new_robot->location());
        const auto result = m_robots.emplace(std::move(new_robot->name()), std::move(new_robot));

        std::cout << "\nNumber of robots on the grid: " << m_robots.size() << '\n';

        return result.second;
    }

    void RobotSimulator::impl::report() const noexcept
    {
        if (!isGridEmpty())
        {
            std::cout << "\nFound " << m_robots.size() << " robots on the grid\n";
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
            for (auto& [name, robot] : m_robots)
            {
                const auto current_location = robot->location();
                robot->move();

                if (!m_grid.isOffTheGrid(robot))
                {
                    std::cout << '\n' << name << " moved one unit forward heading " 
                                << robot->location().direction << "("
                                << robot->location().x_coordinate << ","
                                << robot->location().y_coordinate << ")\n";

                    m_grid.updateLocation(current_location, robot->location(), robot->Id());
                }
                else
                {
                    std::cout << '\n' << robot->name() << " is already at the edge of the grid, facing " 
                              << robot->location().direction << "("
                              << current_location.x_coordinate << ","
                              << current_location.y_coordinate << ")\n";

                    // Revert to previous location
                    robot->setLocation(current_location);
                }
            }
        }
    }

    bool RobotSimulator::impl::move(const RobotParameters& robot) noexcept 
    {
        bool result{false};

        if (!isGridEmpty()) 
        {
            if (auto search = m_robots.find(robot.name); search != m_robots.end())
            {
                const auto current_location = search->second->location();

                search->second->move(robot.pace);

                // Checks if location is outside the grid or is occupied by another robot
                if (!m_grid.isOffTheGrid(search->second) && !m_grid.isOccupied(search->second))
                {
                  std::cout << '\n' << search->second->name() << " moved " << robot.pace << " pace(s) forward heading " 
                            << search->second->location().direction << "("
                            << search->second->location().x_coordinate << ","
                            << search->second->location().y_coordinate << ")\n";
                    
                    m_grid.updateLocation(current_location, search->second->location(), search->second->Id());
                    result = true;
                } 
                else 
                {
                    std::cout << '\n' << search->second->name() << " is unable to move. Area is occupied or outside the grid.\n";
                    
                    // Revert to previous location
                    search->second->setLocation(current_location);
                 }
            }
            else 
            {
                std::cout << robot.name << " isn't on the grid!\n";
            }
        }

        return result;
     
    }

    void RobotSimulator::impl::rotateLeft() noexcept
    {
        if (!isGridEmpty())
        {
            for (auto& [name, robot] : m_robots)
            {
                robot->rotate();
                std::cout << '\n'
                          << name << " turned left facing "
                          << robot->location().direction << "("
                          << robot->location().x_coordinate << ","
                          << robot->location().y_coordinate << ")\n";
            }
        }
    }

    bool RobotSimulator::impl::rotateLeft(const RobotParameters& robot) noexcept 
    {
        bool result {false};

        if (!isGridEmpty()) 
        {
            if (auto search = m_robots.find(robot.name); search != m_robots.end())
            {
                search->second->rotate();

                std::cout << '\n' << search->second->name() << " turned left facing " 
                          << search->second->location().direction << "("
                          << search->second->location().x_coordinate << ","
                          << search->second->location().y_coordinate << ")\n";

                result = true;
            } 
            else 
            {
                std::cout << robot.name << " isn't on the grid!\n";
            }
        }
        
        return result;
    }

    void RobotSimulator::impl::rotateRight() noexcept
    {
        if (!isGridEmpty())
        {
            for (auto& [name, robot] : m_robots)
            {
                robot->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);

                std::cout << '\n'
                          << name << " shifted right facing "
                          << robot->location().direction << "("
                          << robot->location().x_coordinate << ","
                          << robot->location().y_coordinate << ")\n";
            }
        }
    }

    bool RobotSimulator::impl::rotateRight(const RobotParameters& robot) noexcept 
    {
        bool result {false};

        if (!isGridEmpty()) 
        {
            if (auto search = m_robots.find(robot.name); search != m_robots.end()) 
            {
                search->second->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);

                std::cout << '\n' << search->second->name() << " turned right facing "
                          << search->second->location().direction << "("
                          << search->second->location().x_coordinate << ","
                          << search->second->location().y_coordinate << ")\n";

                result = true;
            }
            else 
            {
                std::cout << robot.name << " isn't on the grid!\n";
            }
        }

        return result;
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
        }
    }

    bool RobotSimulator::impl::remove(const RobotParameters& robot) noexcept 
    {
        bool result {false};

        if (auto search = m_robots.find(robot.name); search != m_robots.end()) 
        {
            m_grid.remove(search->second);

            std::cout << "\nThe following robot was removed.\n";
            Menu::showDetails(search->second);
            
            m_robots.erase(robot.name);

            result = true;
        }
        else 
        {
            std::cout << robot.name << " isn't on the grid!\n";
        }

        return result;
    }

    

    RobotSimulator::RobotSimulator(RobotGrid& grid) noexcept
        : m_pImpl{std::make_unique<impl>(grid)}
    {
    }

    RobotSimulator::~RobotSimulator() = default;

    void RobotSimulator::start() noexcept 
    { 
        m_pImpl->start();
    }

    bool RobotSimulator::place(const RobotParameters& robot) noexcept
    {
        return m_pImpl->place(robot);
    }

    void RobotSimulator::report() const noexcept
    {
        m_pImpl->report();
    }

    void RobotSimulator::move() noexcept
    {
        m_pImpl->move();
    }

    bool RobotSimulator::move(const RobotParameters& robot) noexcept 
    { 
        return m_pImpl->move(robot); 
    }

    void RobotSimulator::rotateLeft() noexcept
    {
        m_pImpl->rotateLeft();
    }

    bool RobotSimulator::rotateLeft(const RobotParameters& robot) noexcept 
    {
        return m_pImpl->rotateLeft(robot); 
    }

    void RobotSimulator::rotateRight() noexcept
    {
        m_pImpl->rotateRight();
    }

    bool RobotSimulator::rotateRight(const RobotParameters& robot) noexcept
    { 
        return m_pImpl->rotateRight(robot);
    }

    void RobotSimulator::removeAll() noexcept 
    { 
        m_pImpl->removeAll();
    }

    bool RobotSimulator::remove(const RobotParameters& robot) noexcept
    {
        return m_pImpl->remove(robot);
    }
}