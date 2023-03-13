#pragma once
#include <memory>


#include "Robot.h"

namespace RobotWorldSimulator{
	
	struct GridSize {
		size_t width;
		size_t height;
	};

	class RobotGrid {
	public:
		RobotGrid(size_t width = 5, size_t height = 5) noexcept;
		RobotGrid(const RobotGrid&) noexcept;
		RobotGrid& operator=(const RobotGrid&) noexcept;
		virtual ~RobotGrid() noexcept;

		void addRobot(std::shared_ptr<RobotFactory::Robot> robot) noexcept;
		void updateLocation(const RobotFactory::RobotLocation& prev_location, std::shared_ptr<RobotFactory::Robot> robot) noexcept;
		[[nodiscard]] bool isOffTheGrid(const RobotFactory::RobotLocation& location) const noexcept;
		[[nodiscard]] const GridSize& getGridSize() const noexcept;

	private:
		struct impl;
		std::shared_ptr<impl> m_pImpl;
	};
}
