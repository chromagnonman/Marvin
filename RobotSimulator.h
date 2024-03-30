#ifndef ROBOTSIMULATOR_H
#define ROBOTSIMULATOR_H

#include <memory>

#include "Marvin.h"
#include "RobotGrid.h"

namespace Simulator {

    namespace COMMAND {
        static constexpr auto PLACE  = "PLACE";
        static constexpr auto MOVE   = "MOVE";
        static constexpr auto LEFT   = "LEFT";
        static constexpr auto RIGHT  = "RIGHT";
        static constexpr auto REPORT = "REPORT";
        static constexpr auto REMOVE = "REMOVE";
        static constexpr auto MENU   = "MENU";
    }

    class RobotSimulator {
    public:
        RobotSimulator() noexcept;
        RobotSimulator(GridSize&& grid) noexcept;
        virtual ~RobotSimulator();

        void start() noexcept;

        bool place(const RobotFactory::Marvin&) noexcept;
        void move() noexcept;
        void rotateLeft() noexcept;
        void rotateRight() noexcept;
        void removeAll() noexcept;
        void report() const noexcept;

        // Individual robot commands
        bool move(const RobotFactory::Marvin&, size_t blocks=1) noexcept;
        bool rotateLeft(const RobotFactory::Marvin&) noexcept;
        bool rotateRight(const RobotFactory::Marvin&) noexcept;
        bool remove(const RobotFactory::Marvin&) noexcept;

    private:
        struct impl;
        std::unique_ptr<impl> m_pImpl;
    };
}

#endif
