#ifndef ROBOTSIMULATOR_H
#define ROBOTSIMULATOR_H

#include <memory>

#include "Robot.h"
#include "RobotGrid.h"

namespace RobotWorldSimulator {

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
        explicit RobotSimulator(RobotGrid& world) noexcept;
        virtual ~RobotSimulator();

        void place(const RobotParameters& robot) noexcept;
        void report() const noexcept;
        void move() noexcept;
        void rotateLeft() noexcept;
        void rotateRight() noexcept;
        void removeAll() noexcept;
        
        // Invidual robot commands
        void move(const RobotParameters&) noexcept;
        void rotateLeft(const RobotParameters&) noexcept;
        void rotateRight(const RobotParameters&) noexcept;
        void remove(const RobotParameters&) noexcept;

        const std::unique_ptr<RobotFactory::Robot>& getRobot(
            const RobotFactory::RobotLocation& location) const noexcept;
        const std::unique_ptr<RobotFactory::Robot>& getRobot(
            const std::string robot_name) const noexcept;

    private:
        struct impl;
        std::unique_ptr<impl> m_pImpl;
    };
}

#endif
