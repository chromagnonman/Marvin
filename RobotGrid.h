#ifndef ROBOTGRID_H
#define ROBOTGRID_H

#include <memory>

#include "RobotGrid.h"
#include "Robot.h"

namespace Simulator{
    
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
        RobotGrid() noexcept;
        RobotGrid(GridSize grid) noexcept;

        RobotGrid(const RobotGrid&) = delete;
        RobotGrid& operator=(const RobotGrid&) = delete;
        
        virtual ~RobotGrid() noexcept;

        bool addRobot(const std::unique_ptr<RobotFactory::Robot>&) noexcept;

        void updateLocation(
            const RobotFactory::RobotLocation&,
            const std::unique_ptr<RobotFactory::Robot>&) noexcept;

        void resize(GridSize&&) noexcept;
        void remove(const std::unique_ptr<RobotFactory::Robot>& robot) noexcept;

        [[nodiscard]] const GridSize& getSize() const noexcept;
        [[nodiscard]] size_t getRobotID(const RobotFactory::RobotLocation&) const noexcept;
        [[nodiscard]] bool isOffTheGrid(const std::unique_ptr<RobotFactory::Robot>&) const noexcept;
        [[nodiscard]] bool isOccupied(const std::unique_ptr<RobotFactory::Robot>&) const noexcept;

    private:
        struct impl;
        std::unique_ptr<impl> m_pImpl;
    };
}

#endif
