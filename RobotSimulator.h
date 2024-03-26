#pragma once

#include <memory>

#include "Robot.h"
#include "RobotGrid.h"

namespace RobotWorldSimulator {

    namespace COMMAND {
        static constexpr auto PLACE = "PLACE";
        static constexpr auto MOVE = "MOVE";
        static constexpr auto LEFT = "LEFT";
        static constexpr auto RIGHT = "RIGHT";
        static constexpr auto REPORT = "REPORT";
    }

    class RobotSimulator {
    public:
        explicit RobotSimulator(RobotGrid& world) noexcept;
        virtual ~RobotSimulator();

        void place(const RobotFactory::RobotLocation& location, const std::string& robot_name) noexcept;
        void report() const noexcept;
        void move() noexcept;
        void rotateLeft() noexcept;
        void rotateRight() noexcept;
        
        void move(size_t robot_ID) noexcept;
        void rotateLeft(size_t robot_ID) noexcept;
        void rotateRight(size_t robot_ID) noexcept;
        void report(size_t robot_ID) noexcept;

        const std::unique_ptr<RobotFactory::Robot>& getRobot(const RobotFactory::RobotLocation& location) const noexcept;
        const std::unique_ptr<RobotFactory::Robot>& getRobot(size_t robot_id) const noexcept;

        void end(size_t robot_ID) noexcept;

    private:
        struct impl;
        std::unique_ptr<impl> m_pImpl;
    };
}