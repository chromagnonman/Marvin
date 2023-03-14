
#include <vector>
#include <iostream>

#include "RobotGrid.h"


namespace RobotWorldSimulator {

	struct RobotGrid::impl {

		impl(GridSize&& gridSz) noexcept;

		void addRobot(const std::shared_ptr<RobotFactory::Robot>& robot) noexcept;
		void updateLocation(const RobotFactory::RobotLocation& prev_location, const std::shared_ptr<RobotFactory::Robot>& robot) noexcept;
		bool isOffTheGrid(const RobotFactory::RobotLocation& location) const noexcept;
		const GridSize& getGridSize() const noexcept;
		void resizeGrid(const GridSize& gridSz) noexcept;
		std::shared_ptr<RobotFactory::Robot> getRobot(const RobotFactory::RobotLocation& location) const noexcept;

	private:
		std::vector<std::vector<std::shared_ptr<RobotFactory::Robot>>> m_grid;
		GridSize m_gridSz;

	};

	void RobotGrid::impl::resizeGrid(const GridSize& gridSz) noexcept
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
		resizeGrid(gridSz);
	}

	bool RobotGrid::impl::isOffTheGrid(const RobotFactory::RobotLocation& location) const noexcept
	{
		return location.x_coordinate > m_gridSz.width || location.y_coordinate > m_gridSz.height
			|| location.x_coordinate < 0 || location.y_coordinate < 0;
	}

	void RobotGrid::impl::addRobot(const std::shared_ptr<RobotFactory::Robot>& robot) noexcept
	{
		const auto location = robot->location();

		if (!isOffTheGrid(location)) // TODO: Also check if location is already occupied by another robot.
		{
			m_grid[location.x_coordinate][location.y_coordinate] = robot;
		}
	}

	void RobotGrid::impl::updateLocation(const RobotFactory::RobotLocation& prev_location, const std::shared_ptr<RobotFactory::Robot>& robot) noexcept
	{
		std::cout << "\nUpdated robot location.\n";
		// TODO: Also check if location is already occupied by another robot.
		m_grid[prev_location.x_coordinate][prev_location.y_coordinate] = nullptr;
		m_grid[robot->location().x_coordinate][robot->location().y_coordinate] = robot;
	}

	std::shared_ptr<RobotFactory::Robot> RobotGrid::impl::getRobot(const RobotFactory::RobotLocation& location) const noexcept
	{
		if (!isOffTheGrid(location))
		{
			return m_grid[location.x_coordinate][location.y_coordinate];
		}

		return nullptr;
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

	void RobotGrid::addRobot(const std::shared_ptr<RobotFactory::Robot>& robot) noexcept
	{
		m_pImpl->addRobot(robot);
	}

	void RobotGrid::updateLocation(const RobotFactory::RobotLocation& prev_location, const std::shared_ptr<RobotFactory::Robot>& robot) noexcept
	{
		m_pImpl->updateLocation(prev_location, robot);
	}

	bool RobotGrid::isOffTheGrid(const RobotFactory::RobotLocation& location) const noexcept
	{
		return m_pImpl->isOffTheGrid(location);
	}

	const GridSize& RobotGrid::getGridSize() const noexcept
	{
		return m_pImpl->getGridSize();
	}

	void RobotGrid::resizeGrid(size_t width, size_t height) noexcept
	{
		m_pImpl->resizeGrid(GridSize{ width, height });
	}

	std::shared_ptr<RobotFactory::Robot> RobotGrid::getRobot(const RobotFactory::RobotLocation& location) const noexcept
	{
		return m_pImpl->getRobot(location);
	}


}