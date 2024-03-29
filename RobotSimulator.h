#ifndef ROBOTSIMULATOR_H
#define ROBOTSIMULATOR_H

#include <memory>

#include "Robot.h"
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

    // Auxilliary robot container
    struct RobotParameters {
        RobotFactory::RobotLocation location;
        std::string command;
        std::string name;
        size_t pace{1}; // movement unit
        std::string subcommand;
    };

    class RobotSimulator {
    public:
        explicit RobotSimulator(RobotGrid& grid) noexcept;
        virtual ~RobotSimulator();

        void start() noexcept;

        bool place(const RobotParameters& robot) noexcept;
        void move() noexcept;
        void rotateLeft() noexcept;
        void rotateRight() noexcept;
        void removeAll() noexcept;
        void report() const noexcept;

        // Invidual robot commands
        bool move(const RobotParameters&) noexcept;
        bool rotateLeft(const RobotParameters&) noexcept;
        bool rotateRight(const RobotParameters&) noexcept;
        bool remove(const RobotParameters&) noexcept;

    private:
        struct impl;
        std::unique_ptr<impl> m_pImpl;
    };
}

#endif
