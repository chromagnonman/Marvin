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
        size_t ID{0};
        std::string name{"Marvin"};
        std::string subcommand;
        size_t block{0}; // movement unit
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

        const std::unique_ptr<RobotFactory::Robot>& getRobot(const RobotFactory::RobotLocation& location) const noexcept;
        const std::unique_ptr<RobotFactory::Robot>& getRobot(size_t robot_id) const noexcept;

    private:
        struct impl;
        std::unique_ptr<impl> m_pImpl;
    };
}

#endif
