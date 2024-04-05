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

    volatile std::sig_atomic_t signal_status;

    void signal_handler(int signal)
    {
        signal_status = signal;
    }

    struct RobotSimulator::impl {

        impl(GridSize&& grid) noexcept;

        void start() noexcept;
        bool place(const RobotFactory::Marvin&) noexcept;
        
        void moveAll() noexcept;
        void rotateAll(const std::string& direction) noexcept;
        void removeAll() noexcept;
        void report() const noexcept;
        void resize(GridSize&& grid) noexcept;

        bool move(const RobotFactory::Marvin&, size_t) noexcept;
        bool rotate(const RobotFactory::Marvin&, const std::string& direction) noexcept;
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

        if (command == "PLACE")
        {
            if (robot.location().x_coordinate >= m_grid->getSize().width || 
                robot.location().y_coordinate >= m_grid->getSize().height) 
            {
                std::cout << "\nLocation is or off the grid! Current location is set to (0,0)\n";
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
        else if (command == "MOVE") 
        {
          const auto params = Utils::getCommandParams(input);

          auto [robot_model, blocks] = params.value();

          // FixME: Support chain of commands i.e. MOVE R2D2 LEFT.
          if ((blocks == "LEFT" || blocks == "RIGHT")) {
            blocks = "1";
          }

          if (!robot_model.empty()) {
            move(robot, std::stoi(blocks));
          } else {
            moveAll();
          }
        } 
        else if (command == "ROTATE")
        {
            const auto params = Utils::getCommandParams(input);

            const auto [variant, direction] = params.value();

            if (variant.empty()) {
                std::cout << "Usage: ROTATE <direction> or ROTATE <robot> <direction>\n";
                return;
            }

            // if no robot is specified
            if (variant == "LEFT" || variant == "RIGHT") 
            {
                rotateAll(direction);
            }
            else // Rotate specified robot
            {
                rotate(robot, direction);
            }
        } 
       
        else if (command == "REMOVE") 
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
        else if (command == "REPORT") 
        {
            report();
        } 
        else if (command == "RESIZE") 
        {
           size_t width {0};
           size_t height {0};

           std::istringstream input_stream {input};
           input_stream >> command >> width >> height;

           if (m_grid->getSize().width < width || m_grid->getSize().height < height) 
           {
               std::cout << "Grid resized from: (" << m_grid->getSize().width
                         << "x" << m_grid->getSize().height << ") to: (" << width
                         << "x" << height << ")\n";
               
               resize(GridSize{width, height});
           }
           else 
           {
              std::cerr << "\nError resizing; value must be larger than the current grid size.\n";
           }
        }
        else if (command == "MENU") 
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
        std::cout << "\nCurrent grid size: (" << m_grid->getSize().width << "x"
                << m_grid->getSize().height << ")\n";

        if (!isGridEmpty())
        {
            std::cout << "\nFound " << m_robots.size() << " robot(s) in the grid\n";
            for (const auto& [_, robot] : m_robots)
            {
                Menu::showDetails(robot);
            }
        }
    }

    void RobotSimulator::impl::resize(GridSize&& grid) noexcept 
    {
        m_grid->resize(std::move(grid));
    }

    void RobotSimulator::impl::moveAll() noexcept
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

                    m_grid->updateLocation(current_location, robot);
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
                    
                        m_grid->updateLocation(current_location, robot->second);
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

    void RobotSimulator::impl::rotateAll(const std::string& direction) noexcept
    {
        if (!isGridEmpty())
        {
            const auto dir = (direction == "LEFT") ? ROBOT_ROTATION::LEFT : ROBOT_ROTATION::RIGHT;
            for (auto& [model, robot] : m_robots)
            {
                robot->rotate(dir);
                std::cout << '\n'
                          << model << " turned " << direction << " facing "
                          << robot->location().direction << "("
                          << robot->location().x_coordinate << ","
                          << robot->location().y_coordinate << ")\n";
            }
        }
    }

    bool RobotSimulator::impl::rotate(const RobotFactory::Marvin& target_robot, const std::string& direction) noexcept 
    {
        bool result {false};

        if (!isGridEmpty()) 
        {
             const auto dir = (direction == "LEFT") ? ROBOT_ROTATION::LEFT : ROBOT_ROTATION::RIGHT;

            if (auto [robot, last] = m_robots.equal_range(target_robot.model()); robot != m_robots.end())
            {
                result = true;

                for (; robot != last; ++robot) 
                {
                    robot->second->rotate(dir);

                    std::cout << '\n'
                              << robot->second->model() << " turned "
                              << direction << " facing " 
                              << robot->second->location().direction << "("
                              << robot->second->location().x_coordinate << ","
                              << robot->second->location().y_coordinate << ")\n";
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
            for (auto robot = m_robots.begin(); robot != m_robots.end();)
            {
               m_grid->remove(robot->second);
               robot = m_robots.erase(robot);

            }
            std::cout << "\nAll robots were removed!\n";
        }
    }

    bool RobotSimulator::impl::remove(const RobotFactory::Marvin& target_robot) noexcept 
    {
        bool result {false};

        if (auto [robot, last] = m_robots.equal_range(target_robot.model()); robot != m_robots.end()) 
        {
            result = true;

            std::cout << "\nThe following robot(s) were removed. Info:";

            do
            {
                Menu::showDetails(robot->second);

                m_grid->remove(robot->second);
                robot = m_robots.erase(robot);

            } while (robot != last);
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
        m_pImpl->report(); }

    void RobotSimulator::resize(GridSize&& grid) noexcept 
    { 
        m_pImpl->resize(std::move(grid));
    }

    void RobotSimulator::move() noexcept
    {
        m_pImpl->moveAll();
    }

    bool RobotSimulator::move(const RobotFactory::Marvin& robot, size_t paces) noexcept 
    { 
        return m_pImpl->move(robot, paces); 
    }

    bool RobotSimulator::rotate(const RobotFactory::Marvin& robot, const std::string& direction) noexcept
    { 
        return m_pImpl->rotate(robot, direction);
    }

    void RobotSimulator::rotate(const std::string& direction) noexcept 
    {
        return m_pImpl->rotateAll(direction);
    }

    void RobotSimulator::remove() noexcept 
    { 
        m_pImpl->removeAll();
    }

    bool RobotSimulator::remove(const RobotFactory::Marvin& robot) noexcept
    {
        return m_pImpl->remove(robot);
    }
}