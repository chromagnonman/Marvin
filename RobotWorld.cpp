#include "RobotWorld.h"
#include <vector>
#include <iostream>

namespace RobotWorldSimulator {
	struct RobotWorld::impl {

		impl(GridSize&& gridSz);

		void addRobot(const Location& location) noexcept;

		void moveRobot() noexcept;

		bool isOffTheGrid(const Location& location) const noexcept;

		const GridSize& getGridSize() const noexcept;

	private:

		std::vector<std::vector<size_t>> m_world;
		GridSize m_gridSz;
	};

	RobotWorld::impl::impl(GridSize&& gridSz) noexcept : m_gridSz{ gridSz }
	{
	}

	void RobotWorld::impl::addRobot(const Location& location) noexcept
	{
		std::cout << "addRobot\n";
	}

	void RobotWorld::impl::moveRobot() noexcept
	{
		std::cout << "moveRobot\n";
	}

	RobotWorld::RobotWorld(size_t width, size_t length) :
		m_pImpl{ std::make_unique<impl>(GridSize{width, length}) }
	{
	}

	void RobotWorld::addRobot(const Location& location) noexcept
	{
		std::cout << "addRobot\n";
		m_pImpl->addRobot(location);
	}

	void RobotWorld::moveRobot() noexcept
	{
		std::cout << "moveRobot\n";
		m_pImpl->moveRobot();
	}

	bool RobotWorld::isOffTheGrid(const Location& location) const noexcept
	{
		// TODO: check location of the robot
		return m_pImpl->isOffTheGrid(location);
	}

	const GridSize& RobotWorld::getGridSize() const noexcept
	{
		return m_pImpl->getGridSize();
		std::cout << "getGridSize\n";
	}



}