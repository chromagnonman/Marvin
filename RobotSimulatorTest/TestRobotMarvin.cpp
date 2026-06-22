#include "Marvin.h"
#include "Robot.h"

#include <gtest/gtest.h>

namespace
{

TEST(Marvin, MovesAcrossSignedCoordinates)
{
    RobotFactory::Marvin robot{{.x = 0, .y = 0, .direction = RobotFactory::Direction::South}};

    robot.move(2);
    EXPECT_EQ(robot.location().y, -2);

    robot.rotate(RobotFactory::Rotation::Right);
    robot.move(3);
    EXPECT_EQ(robot.location().x, -3);
}

TEST(Marvin, RotatesInBothDirections)
{
    RobotFactory::Marvin robot;

    robot.rotate(RobotFactory::Rotation::Left);
    EXPECT_EQ(robot.location().direction, RobotFactory::Direction::West);

    robot.rotate(RobotFactory::Rotation::Right);
    EXPECT_EQ(robot.location().direction, RobotFactory::Direction::North);
}

TEST(Marvin, AssignsUniqueIds)
{
    const RobotFactory::Marvin first;
    const RobotFactory::Marvin second;

    EXPECT_NE(first.id(), second.id());
}

} // namespace
