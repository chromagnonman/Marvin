#ifndef ROBOTASSEMBLY_H
#define ROBOTASSEMBLY_H

#include <memory>
#include <variant>

#include "Marvin.h"

namespace RobotFactory {

    namespace RobotType {

        enum class Ground_based : size_t 
        {
            Bipedaled,
            Quadrupedaled,
            Wheeled
        };

        enum class Aerial : size_t 
        {
            // TODO:
        };

        enum class Submerged : size_t 
        {
            // TODO:
        };

        enum class Subaquatic : size_t 
        {
            // TODO:
        };
    }

    using ROBOT_TYPE = std::variant<RobotType::Ground_based, 
                                    RobotType::Aerial, 
                                    RobotType::Submerged,
                                    RobotType::Subaquatic>;

    using namespace RobotFactory::RobotType;

    struct RobotAssembly {

        [[nodiscard]] static std::unique_ptr<RobotFactory::Robot> create(
            const ROBOT_TYPE& type,
            const RobotFactory::RobotLocation& location, 
            const std::string& name) 
        {
            const auto robot_type = std::get<0>(type);
            
            switch (robot_type) {
                case Ground_based::Bipedaled:
                            return std::make_unique<RobotFactory::Marvin>(location, name);

                // TODO: Add support for the other robot types
            }
        }
    };
}

#endif
