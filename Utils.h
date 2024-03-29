#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <execution>
#include <string>
#include <sstream>


namespace Simulator {

    struct RobotParameters; // Forward declaration

    class Utils {
        public:

        static void toUpper(std::string& str) noexcept
        {
            std::transform(std::execution::par_unseq, str.begin(), str.end(), str.begin(),
                [](char c) { 
                    return std::toupper(c); 
                });
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

        static void setDirection(std::string& direction) noexcept 
        {
            using namespace RobotFactory::ROBOT_DIRECTION;

            toUpper(direction);

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

        static void processInputParams(RobotParameters& robot, std::string& command) noexcept 
        {
            removeChars(command);

            // Extract the command from the input stream including the parameters
            std::istringstream input_stream{command};
            input_stream >> robot.command >> robot.name >> robot.location.x_coordinate >> robot.location.y_coordinate 
                         >> robot.location.direction;

            toUpper(robot.command);
            setDirection(robot.location.direction);
        }

        static void processCommandParams(RobotParameters& robot, std::string& command) noexcept 
        {
            // Extract the command from the input stream including the parameters
            std::istringstream input_stream{command};

            // For commands such as Move R2D2 2 left - Robot will move two paces to the left.
            input_stream >> robot.command >> robot.name >> robot.pace >> robot.subcommand;
        }
    };
}

#endif
