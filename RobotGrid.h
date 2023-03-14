#pragma once
#include <memory>

#include "RobotGrid.h"
#include "Robot.h"

namespace RobotWorldSimulator{
	
	struct GridSize {
		size_t width;
		size_t height;
	};

	static constexpr size_t DEFAULT_WIDTH = 10;
	static constexpr size_t DEFAULT_HEIGHT = 10;
	static constexpr size_t DEFAULT_NUMBER_OF_ROBOTS = 10;

	/**
	* @brief Provides a 2D grid environment for Marvin - the paranoid android to explore.
	*/
	class RobotGrid {
	public:
		RobotGrid(size_t width = DEFAULT_WIDTH, size_t height = DEFAULT_HEIGHT) noexcept;
		RobotGrid(const RobotGrid&) noexcept;
		RobotGrid& operator=(const RobotGrid&) noexcept;
		virtual ~RobotGrid() noexcept;

		void addRobot(const std::shared_ptr<RobotFactory::Robot>& robot) noexcept;

		void updateLocation(const RobotFactory::RobotLocation& prev_location, const std::shared_ptr<RobotFactory::Robot>& robot) noexcept;

		void resize(size_t width, size_t height) noexcept;

		[[nodiscard]] bool isOffTheGrid(const RobotFactory::RobotLocation& location) const noexcept;

		[[nodiscard]] const GridSize& getSize() const noexcept;

		[[nodiscard]] std::shared_ptr<RobotFactory::Robot> getRobot(const RobotFactory::RobotLocation& location) const noexcept;

	private:
		struct impl;
		std::shared_ptr<impl> m_pImpl;
	};
}
