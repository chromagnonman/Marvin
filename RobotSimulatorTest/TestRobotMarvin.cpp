#include "pch.h"
#include "Marvin.h"

TEST(Marvin, RobotCreationAndMovement)
{
    const RobotFactory::RobotLocation location{ 0, 0, "NORTH" };
    const auto marvin = std::make_shared<RobotFactory::Marvin>(location, "Marvin");

    marvin->move(1);

    EXPECT_GT(marvin->location().y_coordinate, 0);

    // Rotate to the right facing east
    marvin->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);
    
    marvin->move(1);

    EXPECT_GT(marvin->location().x_coordinate, 0);

    std::shared_ptr<RobotFactory::Robot> robot{ marvin };
    EXPECT_EQ(robot->model(), marvin->model());

    robot->rotate(RobotFactory::ROBOT_ROTATION::LEFT); // Rotate left facing north
    robot->move(1);   // Move up one unit
    EXPECT_EQ(robot->location().y_coordinate, marvin->location().y_coordinate);
    EXPECT_EQ(robot->Id(), marvin->Id());
}
