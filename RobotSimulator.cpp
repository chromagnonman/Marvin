#include <iostream>
#include <vector>
#include <csignal>
#include <unordered_map>

#include "RobotSimulator.h"
#include "Marvin.h"
#include "Menu.h"
#include "Utils.h"

namespace Simulator {

    using namespace RobotFactory;
    using namespace Simulator::COMMAND;

    volatile std::sig_atomic_t signal_status;

    void signal_handler(int signal)
    {
        signal_status = signal;
    }

    struct RobotSimulator::impl {

        impl(GridSize&& grid) noexcept;

        void start() noexcept;
        bool place(const RobotFactory::Marvin&) noexcept;
        
        // Affects all robots in the grid
        void move() noexcept;
        void rotateLeft() noexcept;
        void rotateRight() noexcept;
        void removeAll() noexcept;
        void report() const noexcept;


        // Individual robot commands
        bool move(const RobotFactory::Marvin&, size_t) noexcept;
        bool rotateLeft(const RobotFactory::Marvin&) noexcept;
        bool rotateRight(const RobotFactory::Marvin&) noexcept;
        bool remove(const RobotFactory::Marvin&) noexcept;

        // Private
        std::unique_ptr<RobotGrid> m_grid;
        std::unordered_multimap<std::string, std::unique_ptr<RobotFactory::Robot>> m_robots;

        bool isGridEmpty() const noexcept;
        void execute(std::string&& command) noexcept;

    };

    RobotSimulator::impl::impl(GridSize&& grid) noexcept
        : m_grid{std::make_unique<RobotGrid>(grid)}
    {
        m_robots.reserve(grid.height * grid.width);
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

        std::string input;
        Menu::showUsage();

        while (std::getline(std::cin, input)) {

            execute(std::move(input));
        }
    }

    void RobotSimulator::impl::execute(std::string&& input) noexcept
    {
        RobotFactory::Marvin robot;
        std::string command;

        Utils::setCommand(input, robot, command);

        if (command == PLACE)
        {
            //  If XY is greater than or equal to the default size, reset the
            //  location to zero.
            if (robot.location().x_coordinate >= m_grid->getSize().width || 
                robot.location().y_coordinate >= m_grid->getSize().height) 
            {
                RobotLocation new_location{robot.location()};
                
                new_location.x_coordinate = 0;
                new_location.y_coordinate = 0;

                robot.setLocation(new_location);
            }

            // Handle user error on not specifying a model
            if (robot.model().empty()) 
            {
                robot.setModel("R2D2");
            }

            place(robot);
        }
        else if (command == LEFT)
        {
            if (!robot.model().empty()) 
            {
                rotateLeft(robot);
            }
            else 
            {
                rotateLeft();
            }
        }
        else if (command == RIGHT)
        {
            if (!robot.model().empty()) 
            {
                rotateRight(robot);
            } 
            else 
            {
                rotateRight();
            }
        }
        else if (command == MOVE)
        {
            const auto params = Utils::getCommandParams(input);

            const auto [robot_model, blocks] = params.value();
            
            if (!robot_model.empty()) 
            {
                move(robot, blocks);
            } 
            else 
            {
              move();
            }
        }
        else if (command == REMOVE) 
        {
            if (!robot.model().empty()) 
            {
                remove(robot);
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

    bool RobotSimulator::impl::place(const RobotFactory::Marvin& robot) noexcept
    {
        std::unique_ptr<Robot> new_robot = std::make_unique<Marvin>(robot);

        if (!m_grid->addRobot(new_robot))
        {
            std::cout << "\nLocation is occupied or off the grid!\n";

            // Reset location to (0,0)
            RobotLocation new_location {new_robot->location()};
            new_location.x_coordinate = 0;
            new_location.y_coordinate = 0;

            new_robot->setLocation(new_location);
            
            // Don't insert if location (0,0) is also occupied
            if (!m_grid->addRobot(new_robot)) 
            {
                return false;
            }
        }

        std::cout << "\nRobot created. Info:";
        Menu::showDetails(new_robot);
        m_robots.emplace(std::move(robot.model()), std::move(new_robot));

        return true;
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
            for (auto& [model, robot] : m_robots)
            {
                const auto current_location = robot->location();
                
                robot->move();

                if (!m_grid->isOffTheGrid(robot))
                {
                    std::cout << '\n' << model << " moved one unit forward heading " 
                                << robot->location().direction << "("
                                << robot->location().x_coordinate << ","
                                << robot->location().y_coordinate << ")\n";

                    m_grid->updateLocation(current_location, robot->location(), robot->Id());
                }
                else
                {
                    std::cout << '\n' << robot->model() << " is already at the edge of the grid, facing " 
                              << robot->location().direction << "("
                              << current_location.x_coordinate << ","
                              << current_location.y_coordinate << ")\n";

                    // Revert to previous location
                    robot->setLocation(current_location);
                }
            }
        }
    }

    bool RobotSimulator::impl::move(const RobotFactory::Marvin& target_robot,
                                    size_t blocks) noexcept 
    {
        bool result{false};

        if (!isGridEmpty()) 
        {
            if (auto [robot, last] = m_robots.equal_range(target_robot.model());
                  robot != m_robots.end())
            {
                for (; robot != last; ++robot) 
                {
                    const auto current_location = robot->second->location();

                    robot->second->move(blocks);

                    // Checks if location is outside the grid or is occupied by another robot
                    if (!m_grid->isOffTheGrid(robot->second) && !m_grid->isOccupied(robot->second))
                    {
                        std::cout << '\n' << robot->second->model() << " moved " << blocks << " block(s) forward heading " 
                                << robot->second->location().direction << "("
                                << robot->second->location().x_coordinate << ","
                                << robot->second->location().y_coordinate << ")\n";
                    
                        m_grid->updateLocation(current_location, robot->second->location(), robot->second->Id());
                        result = true;
                    } 
                    else 
                    {
                        std::cout << '\n' << robot->second->model() << " is unable to move. Area is occupied or outside the grid.\n";
                    
                        // Revert to previous location
                        robot->second->setLocation(current_location);
                     }
                }
            }
            else 
            {
                std::cout << target_robot.model() << " isn't on the grid!\n";
            }
        }

        return result;
    }

    void RobotSimulator::impl::rotateLeft() noexcept
    {
        if (!isGridEmpty())
        {
            for (auto& [model, robot] : m_robots)
            {
                robot->rotate();
                std::cout << '\n'
                          << model << " turned left facing "
                          << robot->location().direction << "("
                          << robot->location().x_coordinate << ","
                          << robot->location().y_coordinate << ")\n";
            }
        }
    }

    bool RobotSimulator::impl::rotateLeft(const RobotFactory::Marvin& target_robot) noexcept 
    {
        bool result {false};

        if (!isGridEmpty()) 
        {
            if (auto [robot, last] = m_robots.equal_range(target_robot.model()); robot != m_robots.end())
            {
                for (; robot != last; ++robot) 
                {
                    robot->second->rotate();

                    std::cout << '\n' << robot->second->model() << " turned left facing " 
                              << robot->second->location().direction << "("
                              << robot->second->location().x_coordinate << ","
                              << robot->second->location().y_coordinate << ")\n";

                    result = true;
                }
            } 
            else 
            {
                std::cout << target_robot.model() << " isn't on the grid!\n";
            }
        }
        
        return result;
    }

    void RobotSimulator::impl::rotateRight() noexcept
    {
        if (!isGridEmpty())
        {
            for (auto& [model, robot] : m_robots)
            {
                robot->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);

                std::cout << '\n'
                          << model << " shifted right facing "
                          << robot->location().direction << "("
                          << robot->location().x_coordinate << ","
                          << robot->location().y_coordinate << ")\n";
            }
        }
    }

    bool RobotSimulator::impl::rotateRight(const RobotFactory::Marvin& target_robot) noexcept 
    {
        bool result {false};

        if (!isGridEmpty()) 
        {
            if (auto [robot, last] = m_robots.equal_range(target_robot.model()); robot != m_robots.end()) 
            {
                for (; robot != last; ++robot) 
                {
                    robot->second->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);

                    std::cout << '\n' << robot->second->model() << " turned right facing "
                              << robot->second->location().direction << "("
                              << robot->second->location().x_coordinate << ","
                              << robot->second->location().y_coordinate << ")\n";

                    result = true;
                }
            }
            else 
            {
                std::cout << target_robot.model() << " isn't on the grid!\n";
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
               m_grid->remove(robot.second);
            }

            m_robots.clear();
            std::cout << "\nAll robots were terminated!\n";
        }
    }

    bool RobotSimulator::impl::remove(const RobotFactory::Marvin& target_robot) noexcept 
    {
        bool result {false};

        if (auto [robot, last] = m_robots.equal_range(target_robot.model()); robot != m_robots.end()) 
        {
            for (auto first = robot; first != last; ++first) 
            {
                m_grid->remove(first->second);

                std::cout << "\nRobot was removed. Info:";
                Menu::showDetails(first->second);
            }

          m_robots.erase(robot, last);

          result = true;
        }
        else 
        {
            std::cout << target_robot.model() << " isn't on the grid!\n";
        }

        return result;
    }

    

    RobotSimulator::RobotSimulator() noexcept
        : m_pImpl{std::make_unique<impl>(GridSize{DEFAULT_HEIGHT, DEFAULT_WIDTH})}
    {
    }

    RobotSimulator::RobotSimulator(GridSize&& grid) noexcept
        : m_pImpl{std::make_unique<impl>(std::move(grid))} 
    {
    }

    RobotSimulator::~RobotSimulator() = default;

    void RobotSimulator::start() noexcept 
    { 
        m_pImpl->start();
    }

    bool RobotSimulator::place(const RobotFactory::Marvin& robot) noexcept
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

    bool RobotSimulator::move(const RobotFactory::Marvin& robot, size_t paces) noexcept 
    { 
        return m_pImpl->move(robot, paces); 
    }

    void RobotSimulator::rotateLeft() noexcept
    {
        m_pImpl->rotateLeft();
    }

    bool RobotSimulator::rotateLeft(const RobotFactory::Marvin& robot) noexcept 
    {
        return m_pImpl->rotateLeft(robot); 
    }

    void RobotSimulator::rotateRight() noexcept
    {
        m_pImpl->rotateRight();
    }

    bool RobotSimulator::rotateRight(const RobotFactory::Marvin& robot) noexcept
    { 
        return m_pImpl->rotateRight(robot);
    }

    void RobotSimulator::removeAll() noexcept 
    { 
        m_pImpl->removeAll();
    }

    bool RobotSimulator::remove(const RobotFactory::Marvin& robot) noexcept
    {
        return m_pImpl->remove(robot);
    }
}