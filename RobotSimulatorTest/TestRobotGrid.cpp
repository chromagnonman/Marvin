#include "Marvin.h"
#include "Robot.h"
#include "RobotGrid.h"

#include <gtest/gtest.h>

namespace
{

TEST(RobotGrid, SupportsRectangularGrids)
{
    Simulator::RobotGrid grid{{.width = 3, .height = 5}};
    const RobotFactory::Marvin robot{{.x = 2, .y = 4, .direction = RobotFactory::Direction::North}};

    EXPECT_TRUE(grid.addRobot(robot));
    EXPECT_EQ(grid.robotIdAt(robot.location()), robot.id());
}

TEST(RobotGrid, RejectsNegativeCoordinates)
{
    Simulator::RobotGrid grid;
    const RobotFactory::Marvin robot{
        {.x = -1, .y = 0, .direction = RobotFactory::Direction::North}};

    EXPECT_TRUE(grid.isOffGrid(robot.location()));
    EXPECT_FALSE(grid.addRobot(robot));
}

TEST(RobotGrid, RejectsOccupiedCells)
{
    Simulator::RobotGrid grid;
    const RobotFactory::Marvin first{{.x = 1, .y = 1, .direction = RobotFactory::Direction::North}};
    const RobotFactory::Marvin second{
        {.x = 1, .y = 1, .direction = RobotFactory::Direction::South}};

    ASSERT_TRUE(grid.addRobot(first));
    EXPECT_FALSE(grid.addRobot(second));
}

TEST(RobotGrid, PreservesRobotsWhenExpanded)
{
    Simulator::RobotGrid grid{{.width = 2, .height = 2}};
    const RobotFactory::Marvin robot{{.x = 1, .y = 1, .direction = RobotFactory::Direction::North}};
    ASSERT_TRUE(grid.addRobot(robot));

    ASSERT_TRUE(grid.resize({.width = 4, .height = 3}));
    EXPECT_EQ(grid.robotIdAt(robot.location()), robot.id());
}

TEST(RobotGrid, RejectsShrinking)
{
    Simulator::RobotGrid grid{{.width = 4, .height = 3}};

    EXPECT_FALSE(grid.resize({.width = 3, .height = 3}));
    EXPECT_EQ(grid.size().width, 4);
}

} // namespace
