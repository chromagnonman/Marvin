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

    /**
    * @brief Provides a 2D grid environment for Marvin (the paranoid android) to explore.
    */
    class RobotGrid {
    public:
        RobotGrid(size_t width = DEFAULT_WIDTH, size_t height = DEFAULT_HEIGHT) noexcept;

        RobotGrid(const RobotGrid&) = delete;
        RobotGrid& operator=(const RobotGrid&) = delete;
        
        virtual ~RobotGrid() noexcept;

        void addRobot(
                    size_t robotId,
                    const RobotFactory::RobotLocation& location) noexcept;

        void updateLocation(
                    const RobotFactory::RobotLocation& prev_location,
                    const RobotFactory::RobotLocation& location,
                    size_t robotId) noexcept;

        void resize(size_t width, size_t height) noexcept;

        [[nodiscard]] const GridSize& getSize() const noexcept;

        [[nodiscard]] size_t getRobotID(const RobotFactory::RobotLocation& location) const noexcept;

        [[nodiscard]] bool isOffTheGrid(const std::unique_ptr<RobotFactory::Robot>& robot) const noexcept;

    private:
        struct impl;
        std::unique_ptr<impl> m_pImpl;
    };
}
