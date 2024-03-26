#include "pch.h"
#include "RobotGrid.h"
#include "Marvin.h"

using Robot = std::unique_ptr<RobotFactory::Robot>;

TEST(RobotGridTest, DefaultGridSize) {

	RobotWorldSimulator::RobotGrid grid;

	EXPECT_EQ(grid.getSize().height, RobotWorldSimulator::DEFAULT_HEIGHT);
	EXPECT_EQ(grid.getSize().width, RobotWorldSimulator::DEFAULT_WIDTH);

	RobotFactory::RobotLocation location { 11, 11, "NORTH" };

	Robot R2 = std::make_unique<RobotFactory::Marvin>(location);

	EXPECT_TRUE(grid.isOffTheGrid(R2));
}

TEST(RobotGridTest, CustomGridSize) {

	RobotWorldSimulator::RobotGrid grid{ 5,5 };

	EXPECT_EQ(grid.getSize().height, 5);
	EXPECT_EQ(grid.getSize().width, 5);

	RobotFactory::RobotLocation location{11, 11, "NORTH"};
    Robot R1 = std::make_unique<RobotFactory::Marvin>(location);

	EXPECT_TRUE(grid.isOffTheGrid(R1));

	RobotFactory::RobotLocation location2{4, 4, "NORTH"};
    Robot R2 = std::make_unique<RobotFactory::Marvin>(location2);

	EXPECT_FALSE(grid.isOffTheGrid(R2));
}

TEST(RobotGridTest, ResizeGrid) {

	// Initialize to 5 X 5
	RobotWorldSimulator::RobotGrid grid{ 5,5 };

	EXPECT_EQ(grid.getSize().height, 5);
	EXPECT_EQ(grid.getSize().width, 5);

	RobotFactory::RobotLocation location{11, 11, "NORTH"};
    Robot R1 = std::make_unique<RobotFactory::Marvin>(location);

	EXPECT_TRUE(grid.isOffTheGrid(R1));

	grid.resize(10, 10);

    RobotFactory::RobotLocation location2{11, 11, "NORTH"};
    Robot R2 = std::make_unique<RobotFactory::Marvin>(location2);

	EXPECT_FALSE(grid.isOffTheGrid(R2));
}

TEST(RobotGridTest, PullRobotFromTheGrid)
{
	RobotWorldSimulator::RobotGrid grid;

	RobotFactory::RobotLocation location{ 0, 0, "NORTH" };
	const std::string name{ "NEO" };

	Robot R1 = std::make_unique<RobotFactory::Marvin>(location, name);

	grid.addRobot(R1->Id(), R1->location());

	auto robot_id = grid.getRobotID(location);
	EXPECT_GT(robot_id, 0);
}

TEST(TestRobotGrid, RobotOffTheGrid)
{
	RobotWorldSimulator::RobotGrid grid; // Default size is 10x10
	RobotFactory::RobotLocation location{ 20, 20, "NORTH" };

	Robot R1 = std::make_unique<RobotFactory::Marvin>(location);
		
	EXPECT_TRUE(grid.isOffTheGrid(R1));

	// Robot's location should be set to 0
	EXPECT_EQ(R1->location().x_coordinate, 0);
	EXPECT_EQ(R1->location().y_coordinate, 0);
		
	R1->move(); // Move up Y should be incremented
	EXPECT_FALSE(grid.isOffTheGrid(R1));
	EXPECT_GT(R1->location().y_coordinate, 0);
}
