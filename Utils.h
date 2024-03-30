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

        static void setCommand(std::string& input, RobotFactory::Robot& robot, std::string& command) noexcept 
        {
            removeChars(input);
            RobotFactory::RobotLocation location;
            std::string model;

            std::istringstream input_stream {input};

            input_stream >> command >> model >> location.x_coordinate >>
                location.y_coordinate >> location.direction;

            toUpper(command);
            toUpper(location.direction);

            robot.setLocation(location);
            robot.setModel(model);
        }

        static std::optional<std::tuple<std::string, size_t>> getCommandParams(const std::string& input) noexcept 
        {
            // Extract the command from the input stream including the parameters
            std::istringstream input_stream{input};
            std::string com;
            std::string robot_model;
            size_t pace {1};

            // For commands such as Move R2D2 2 left - Robot will move two paces to the left.
            input_stream >> com >> robot_model >> pace;

            return {std::tuple{robot_model, pace}};
        }
    };
}

#endif
