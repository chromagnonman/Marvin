#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <execution>
#include <string>
#include <sstream>
#include <optional>
#include <tuple>


namespace Simulator {

    class Utils {
        public:

        static void toUpper(std::vector<std::reference_wrapper<std::string>> strings) noexcept
        {
           for (auto str : strings) 
           {
            
                std::transform(std::execution::par_unseq, str.get().begin(), str.get().end(), str.get().begin(),
                    [](char c) { 
                        return std::toupper(c); 
                    });
            
           }
        }

        // Replace extraneous characters with spaces
        static void removeChars(std::string& str) noexcept 
        {
            std::replace_if( std::execution::par_unseq, str.begin(), str.end(),
                [](const unsigned char c) noexcept 
                {
                    return !isalnum(c);
                }, ' ');
        }

        // Handle mistyped direction
        static void setDirection(std::string& direction) noexcept 
        {
            using namespace RobotFactory::ROBOT_DIRECTION;

            if (direction == NORTH) 
            {
              direction = NORTH;
            } 
            else if (direction == SOUTH) 
            {
              direction = SOUTH;
            } 
            else if (direction == EAST) 
            {
              direction = EAST;
            } 
            else if (direction == WEST) 
            {
              direction = WEST;
            } 
            else 
            {
              direction = NORTH;  // Default
            }
        }

        static void setCommand(std::string& input, RobotFactory::Marvin& robot, std::string& command) noexcept 
        {
            removeChars(input);

            RobotFactory::RobotLocation location;
            std::string model;

            std::istringstream input_stream {input};

            input_stream >> command >> model >> location.x_coordinate >>
                location.y_coordinate >> location.direction;

            toUpper({command, location.direction});
            setDirection(location.direction);
            
            if (robot != location) 
            {
                robot.setLocation(location);
            }

            if (!model.empty() && model != robot.model()) 
            {
                robot.setModel(model);
            }
        }

        static std::optional<std::tuple<std::string, std::string>> getCommandParams(const std::string& input) noexcept 
        {
            // Extract the command from the input stream including the parameters
            std::istringstream input_stream{input};
            std::string com;
            std::string robot_model;
            std::string variant{"1"}; // set as one unit

            // For commands such as Move R2D2 2 (units) or Rotate R2D2 Left
            input_stream >> com >> robot_model >> variant;

            toUpper({robot_model, variant});

            return {std::tuple{robot_model, variant}};
        }
    };
}

#endif
