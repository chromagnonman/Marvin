#ifndef ROBOT_ASSEMBLY_H
#define ROBOT_ASSEMBLY_H

#include "Marvin.h"
#include "Robot.h"

#include <memory>
#include <string>
#include <utility>

namespace RobotFactory
{

enum class GroundRobotType
{
    Bipedal
};

// Future enhancements:
// - Ground-based: Quadrupedal, Wheeled
// - Aerial robot types
// - Aquatic robot types
// - Subaquatic robot types

struct RobotAssembly
{
    [[nodiscard]] static std::unique_ptr<Robot> create(GroundRobotType type, RobotLocation location,
                                                       std::string name)
    {
        switch (type)
        {
        case GroundRobotType::Bipedal:
            return std::make_unique<Marvin>(location, std::move(name));
        }

        return nullptr;
    }
};

} // namespace RobotFactory

#endif
