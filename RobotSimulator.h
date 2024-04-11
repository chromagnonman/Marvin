#ifndef ROBOTSIMULATOR_H
#define ROBOTSIMULATOR_H

#include <memory>

#include "Marvin.h"
#include "RobotGrid.h"

namespace Simulator {

    class RobotSimulator {
    public:
        RobotSimulator() noexcept;
        RobotSimulator(GridSize&& grid) noexcept;
        virtual ~RobotSimulator();

        void start() noexcept;
        // TODO: Use robot type (enum value) as argument to create the robot
        bool place(std::unique_ptr<RobotFactory::Robot>) noexcept;
        void move() noexcept;
        void rotate(const std::string& direction) noexcept;
        void remove() noexcept;
        void report() const noexcept;
        void resize(GridSize&&) noexcept;

        // Individual robot commands
        bool move(const std::string& robot, size_t blocks=1) noexcept;
        bool rotate(const std::string& robot, const std::string& direction) noexcept;
        bool remove(const std::string& robot) noexcept;

    private:
        struct impl;
        std::unique_ptr<impl> m_pImpl;
    };
}

#endif
