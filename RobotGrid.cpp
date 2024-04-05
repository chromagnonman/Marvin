#include <vector>

#include "RobotGrid.h"
#include "Marvin.h"

namespace Simulator {

    struct RobotGrid::impl {

        impl(GridSize&& gridSz) noexcept;

        bool addRobot(const std::unique_ptr<RobotFactory::Robot>& robot) noexcept;

        void updateLocation(
            const RobotFactory::RobotLocation& location,
            const std::unique_ptr<RobotFactory::Robot>& robot) noexcept;

        const GridSize& getSize() const noexcept;

        void resize(GridSize&& gridSz) noexcept;

        void remove(const std::unique_ptr<RobotFactory::Robot>& robot) noexcept;

        size_t getRobotID(const RobotFactory::RobotLocation& location) const noexcept;
        bool isOffTheGrid(const std::unique_ptr<RobotFactory::Robot>& robot) const noexcept;
        bool isOccupied(const std::unique_ptr<RobotFactory::Robot>& robot) const noexcept;

        // Private data
        using ROBOT_ID = size_t;
        std::vector<std::vector<ROBOT_ID>> m_grid;
        GridSize m_gridSz;

        void initialize() noexcept;
    };

    void RobotGrid::impl::resize(GridSize&& gridSz) noexcept
    {
        m_gridSz = gridSz;

        initialize();
    }

    void RobotGrid::impl::initialize() noexcept 
    {
        m_grid.resize(m_gridSz.height);

        for (size_t i = 0; i < m_gridSz.height; i++) {
            m_grid[i].resize(m_gridSz.width);
        }
    }

    RobotGrid::impl::impl(GridSize&& gridSz) noexcept : m_gridSz{gridSz}
    {
        initialize();
    }

    bool RobotGrid::impl::addRobot(const std::unique_ptr<RobotFactory::Robot>& robot) noexcept
    {
        if (!isOffTheGrid(robot) && !isOccupied(robot))
        {
            m_grid[robot->location().x_coordinate][robot->location().y_coordinate] =
                robot->Id();

            return true;
        }

        return false;
    }

    void RobotGrid::impl::updateLocation(
        const RobotFactory::RobotLocation& location,
        const std::unique_ptr<RobotFactory::Robot>& robot) noexcept
    {
        m_grid[location.x_coordinate][location.y_coordinate] = 0;
        m_grid[robot->location().x_coordinate][robot->location().y_coordinate] = robot->Id();
    }

    size_t RobotGrid::impl::getRobotID(const RobotFactory::RobotLocation& location) const noexcept
    {
        return m_grid[location.x_coordinate][location.y_coordinate];
    }

    const GridSize& RobotGrid::impl::getSize() const noexcept
    {
        return m_gridSz;
    }

    bool RobotGrid::impl::isOffTheGrid(const std::unique_ptr<RobotFactory::Robot>& robot) const noexcept
    {
        return robot->location().x_coordinate >= m_gridSz.width ||
                robot->location().y_coordinate >= m_gridSz.height ||
                robot->location().x_coordinate < 0 ||
                robot->location().y_coordinate < 0;
    }

    bool RobotGrid::impl::isOccupied(const std::unique_ptr<RobotFactory::Robot>& robot) const noexcept
    {
        return m_grid[robot->location().x_coordinate][robot->location().y_coordinate];
    }
    
    void RobotGrid::impl::remove(const std::unique_ptr<RobotFactory::Robot>& robot) noexcept 
    {
        m_grid[robot->location().x_coordinate][robot->location().y_coordinate] = 0;
    }

    RobotGrid::RobotGrid(GridSize grid)  noexcept :
        m_pImpl{ std::make_unique<impl>(std::move(grid)) }
    {
        m_pImpl->initialize();
    }

    RobotGrid::RobotGrid() noexcept : 
        m_pImpl{ std::make_unique<impl>(GridSize{DEFAULT_WIDTH, DEFAULT_HEIGHT}) }
    {
        m_pImpl->initialize();
    }

    RobotGrid::~RobotGrid() = default;


    bool RobotGrid::addRobot(const std::unique_ptr<RobotFactory::Robot>& robot) noexcept
    {
        return m_pImpl->addRobot(robot);
    }
    
    void RobotGrid::updateLocation(
        const RobotFactory::RobotLocation& location, 
        const std::unique_ptr<RobotFactory::Robot>& robot) noexcept
    {
        m_pImpl->updateLocation(location, robot);
    } 

    const GridSize& RobotGrid::getSize() const noexcept
    {
        return m_pImpl->getSize();
    }

    void RobotGrid::resize(GridSize&& grid) noexcept
    {
        m_pImpl->resize(std::move(grid));
    }

    void RobotGrid::remove(const std::unique_ptr<RobotFactory::Robot>& robot) noexcept 
    {
        m_pImpl->remove(robot);
    }

    size_t RobotGrid::getRobotID(const RobotFactory::RobotLocation& location) const noexcept
    {
        return m_pImpl->getRobotID(location);
    }

    bool RobotGrid::isOffTheGrid(const std::unique_ptr<RobotFactory::Robot>& robot) const noexcept 
    {
        return m_pImpl->isOffTheGrid(robot);
    }

    bool RobotGrid::isOccupied(const std::unique_ptr<RobotFactory::Robot>& robot) const noexcept
    {
        return m_pImpl->isOccupied(robot);
    }


 }
