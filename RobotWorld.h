#pragma once
#include <memory>

struct Location;

namespace RobotWorldSimulator{
	
	struct GridSize {
		size_t width;
		size_t height;
	};

	class RobotWorld {
	public:
		RobotWorld(size_t width = 5, size_t height = 5) noexcept;
		virtual ~RobotWorld() noexcept;

		void addRobot(const Location& location) noexcept;
		void moveRobot() noexcept;
		[[nodiscard]] bool isOffTheGrid(const Location& location) const noexcept;
		[[nodiscard]] const GridSize& getGridSize() const noexcept;

	private:
		struct impl;
		std::unique_ptr<impl> m_pImpl;
	};
}
