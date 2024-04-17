#ifndef MARVIN_H
#define MARVIN_H

#include "Robot.h"
#include "RobotGrid.h"

namespace RobotFactory {

    /**
    * @brief Defines a type called Marvin - a chronically depressed robot.
    */
    class Marvin : public Robot {
      public:
        Marvin(const std::string& name = "Marvin") noexcept;
        Marvin(const RobotLocation& location) noexcept;
        Marvin(const RobotLocation&, std::string robot_name) noexcept;
        Marvin(const Marvin&) = default;
        Marvin& operator=(const Marvin&) = default;
        virtual ~Marvin();

        void rotate(const ROBOT_ROTATION&) noexcept override;
        void move(size_t) noexcept override;

      protected:
        const size_t m_default_move = 1;
    };
}

#endif
