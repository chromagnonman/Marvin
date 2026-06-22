#include "pch.h"
#include "RobotGrid.h"
#include "Marvin.h"

using Robot = std::unique_ptr<RobotFactory::Robot>;

TEST(RobotGrid, DefaultGridSize)
{

    Simulator::RobotGrid grid;

    EXPECT_EQ(grid.getSize().height, Simulator::DEFAULT_HEIGHT);
    EXPECT_EQ(grid.getSize().width, Simulator::DEFAULT_WIDTH);

    RobotFactory::RobotLocation location{11, 11, "NORTH"};

    Robot R2 = std::make_unique<RobotFactory::Marvin>(location);

    EXPECT_TRUE(grid.isOffTheGrid(*R2));
}

TEST(RobotGrid, CustomGridSize)
{

    Simulator::RobotGrid grid{{5, 5}};

    EXPECT_EQ(grid.getSize().height, 5);
    EXPECT_EQ(grid.getSize().width, 5);

    RobotFactory::RobotLocation location{11, 11, "NORTH"};
    Robot R1 = std::make_unique<RobotFactory::Marvin>(location);

    EXPECT_TRUE(grid.isOffTheGrid(*R1));

    RobotFactory::RobotLocation location2{4, 4, "NORTH"};
    Robot R2 = std::make_unique<RobotFactory::Marvin>(location2);

    EXPECT_FALSE(grid.isOffTheGrid(*R2));
}

TEST(RobotGrid, ResizeGrid)
{

    // Initialize to 5 X 5
    Simulator::RobotGrid grid{{5, 5}};

    EXPECT_EQ(grid.getSize().height, 5);
    EXPECT_EQ(grid.getSize().width, 5);

    RobotFactory::RobotLocation location{11, 11, "NORTH"};
    Robot R1 = std::make_unique<RobotFactory::Marvin>(location);

    EXPECT_TRUE(grid.isOffTheGrid(*R1));

    grid.resize(Simulator::GridSize{10, 10});

    RobotFactory::RobotLocation location2{5, 5, "NORTH"};
    Robot R2 = std::make_unique<RobotFactory::Marvin>(location2);

    EXPECT_FALSE(grid.isOffTheGrid(*R2));
}

TEST(RobotGrid, PullRobotFromTheGrid)
{
    Simulator::RobotGrid grid;

    RobotFactory::RobotLocation location{0, 0, "NORTH"};
    const std::string name{"NEO"};

    Robot R1 = std::make_unique<RobotFactory::Marvin>(location, name);

    grid.addRobot(*R1);

    auto robot_id = grid.getRobotID(location);
    EXPECT_GT(robot_id, 0);
}

TEST(RobotGrid, RobotOffTheGrid)
{
    Simulator::RobotGrid grid; // Default size is 10x10
    RobotFactory::RobotLocation location{20, 20, "NORTH"};

    Robot R1 = std::make_unique<RobotFactory::Marvin>(location);

    EXPECT_TRUE(grid.isOffTheGrid(*R1));
}

TEST(RobotGrid, SupportsRectangularGrids)
{
    Simulator::RobotGrid grid{{3, 5}};
    Robot robot = std::make_unique<RobotFactory::Marvin>(RobotFactory::RobotLocation{2, 4, "NORTH"},
                                                         "TallGridRobot");

    EXPECT_TRUE(grid.addRobot(*robot));
    EXPECT_EQ(grid.getRobotID(robot->location()), robot->Id());
}

TEST(RobotGrid, PreservesRobotsWhenResized)
{
    Simulator::RobotGrid grid{{2, 2}};
    Robot robot = std::make_unique<RobotFactory::Marvin>(RobotFactory::RobotLocation{1, 1, "NORTH"},
                                                         "ResizeRobot");
    ASSERT_TRUE(grid.addRobot(*robot));

    grid.resize({4, 3});

    EXPECT_EQ(grid.getRobotID(robot->location()), robot->Id());
}
