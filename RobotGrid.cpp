#include <vector>
#include <iostream>

#include "RobotGrid.h"


namespace RobotWorldSimulator {

    using ROBOT_ID = size_t;

    struct RobotGrid::impl {

        impl(GridSize&& gridSz) noexcept;

        void addRobot(
                    size_t robotId,
                    const RobotFactory::RobotLocation& location) noexcept;

        void updateLocation(
                    const RobotFactory::RobotLocation& prev_location,
                    const RobotFactory::RobotLocation& location,
                    size_t robotId) noexcept;

        const GridSize& getSize() const noexcept;

        void resize(const GridSize& gridSz) noexcept;
        void remove(const std::unique_ptr<RobotFactory::Robot>& robot) noexcept;

        size_t getRobotID(const RobotFactory::RobotLocation& location) const noexcept;
        bool isOffTheGrid(const std::unique_ptr<RobotFactory::Robot>& robot) const noexcept;
        bool isOccupied(const std::unique_ptr<RobotFactory::Robot>& robot) const noexcept;

    private:
        std::vector<std::vector<ROBOT_ID>> m_grid;
        GridSize m_gridSz;
    };

    void RobotGrid::impl::resize(const GridSize& gridSz) noexcept
    {
        m_grid.resize(gridSz.height);
        for (size_t i = 0; i < gridSz.height; i++)
        {
            m_grid[i].resize(gridSz.width);
        }

        m_gridSz = gridSz;
    }

    RobotGrid::impl::impl(GridSize&& gridSz) noexcept : m_gridSz{ gridSz }
    {
        // Initialize grid for random access
        resize(gridSz);
    }

    void RobotGrid::impl::addRobot(size_t robotId, const RobotFactory::RobotLocation& location) noexcept
    {
        m_grid[location.x_coordinate][location.y_coordinate] = robotId;
    }

    void RobotGrid::impl::updateLocation(
            const RobotFactory::RobotLocation& prev_location,
            const RobotFactory::RobotLocation& location,
            size_t robotId) noexcept
    {
        m_grid[prev_location.x_coordinate][prev_location.y_coordinate] = 0;
        m_grid[location.x_coordinate][location.y_coordinate] = robotId;
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

    RobotGrid::RobotGrid(size_t width, size_t height)  noexcept :
        m_pImpl{ std::make_unique<impl>(GridSize{width, height}) }
    {
    }

    RobotGrid::RobotGrid() noexcept : 
        m_pImpl{ std::make_unique<impl>(GridSize{DEFAULT_WIDTH, DEFAULT_HEIGHT}) }
    {
    }

    RobotGrid::~RobotGrid() = default;


    void RobotGrid::addRobot(
            size_t robotId,
            const RobotFactory::RobotLocation& location) noexcept
    {
        m_pImpl->addRobot(robotId, location);
    }

    void RobotGrid::updateLocation(
            const RobotFactory::RobotLocation& prev_location,
            const RobotFactory::RobotLocation& location,
            size_t robotId) noexcept
    {
        m_pImpl->updateLocation(prev_location, location, robotId);
    } 

    const GridSize& RobotGrid::getSize() const noexcept
    {
        return m_pImpl->getSize();
    }

    void RobotGrid::resize(size_t width, size_t height) noexcept
    {
        m_pImpl->resize(GridSize{ width, height });
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
