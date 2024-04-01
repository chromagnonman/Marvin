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

        bool place(const RobotFactory::Marvin&) noexcept;
        void move() noexcept;
        void rotate(const std::string& direction) noexcept;
        void remove() noexcept;
        void report() const noexcept;
        void resize(GridSize&&) noexcept;

        // Individual robot commands
        bool move(const RobotFactory::Marvin&, size_t blocks=1) noexcept;
        bool rotate(const RobotFactory::Marvin&, const std::string& direction) noexcept;
        bool remove(const RobotFactory::Marvin&) noexcept;

    private:
        struct impl;
        std::unique_ptr<impl> m_pImpl;
    };
}

#endif
