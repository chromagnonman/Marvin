#include "RobotGrid.h"
#include <vector>
#include <iostream>

namespace RobotWorldSimulator {

	struct RobotGrid::impl {

		impl(GridSize&& gridSz) noexcept;

		void addRobot(std::shared_ptr<RobotFactory::Robot> robot) noexcept;
		void updateLocation(const RobotFactory::RobotLocation& prev_location, std::shared_ptr<RobotFactory::Robot> robot) noexcept;
		bool isOffTheGrid(const RobotFactory::RobotLocation& location) const noexcept;
		const GridSize& getGridSize() const noexcept;

	private:
		std::vector<std::vector<std::shared_ptr<RobotFactory::Robot>>> m_grid;
		GridSize m_gridSz;
	};

	RobotGrid::impl::impl(GridSize&& gridSz) noexcept : m_gridSz{ gridSz }
	{
		// TODO: initialize grid
	}

	bool RobotGrid::impl::isOffTheGrid(const RobotFactory::RobotLocation& location) const noexcept
	{
		return (location.x_coordinate > m_gridSz.width && location.y_coordinate > m_gridSz.height)
			|| (location.x_coordinate < 0 && location.y_coordinate < 0);
	}

	void RobotGrid::impl::addRobot(std::shared_ptr<RobotFactory::Robot> robot) noexcept
	{
		std::cout << "addRobot\n";
		const auto location = robot->location();

		if (!isOffTheGrid(location)) {
			m_grid[location.x_coordinate][location.y_coordinate] = robot;
		}
	}

	void RobotGrid::impl::updateLocation(const RobotFactory::RobotLocation& prev_location, std::shared_ptr<RobotFactory::Robot> robot) noexcept
	{
		std::cout << "Update robot location\n";
		m_grid[prev_location.x_coordinate][prev_location.y_coordinate] = nullptr;
		m_grid[robot->location().x_coordinate][robot->location().y_coordinate] = robot;
	}

	const GridSize& RobotGrid::impl::getGridSize() const noexcept
	{
		return m_gridSz;
	}

	RobotGrid::RobotGrid(size_t width, size_t height)  noexcept :
		m_pImpl{ std::make_unique<impl>(GridSize{width, height}) }
	{
	}

	RobotGrid::RobotGrid(const RobotGrid& grid) noexcept
	{
		if (this != &grid)
		{
			m_pImpl = grid.m_pImpl;
		}
	}

	RobotGrid& RobotGrid::operator=(const RobotGrid& grid) noexcept
	{
		if (this != &grid)
		{
			m_pImpl = grid.m_pImpl;
		}

		return *this;
	}

	RobotGrid::~RobotGrid() = default;

	void RobotGrid::addRobot(std::shared_ptr<RobotFactory::Robot> robot) noexcept
	{
		std::cout << "addRobot\n";
		m_pImpl->addRobot(robot);
	}

	void RobotGrid::updateLocation(const RobotFactory::RobotLocation& prev_location, std::shared_ptr<RobotFactory::Robot> robot) noexcept
	{
		std::cout << "moveRobot\n";
		m_pImpl->updateLocation(prev_location, robot);
	}

	bool RobotGrid::isOffTheGrid(const RobotFactory::RobotLocation& location) const noexcept
	{
		return m_pImpl->isOffTheGrid(location);
	}

	const GridSize& RobotGrid::getGridSize() const noexcept
	{
		return m_pImpl->getGridSize();
		std::cout << "getGridSize\n";
	}



}