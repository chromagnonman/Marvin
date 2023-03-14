#include "pch.h"
#include "RobotGrid.h"
#include "Marvin.h"

TEST(RobotGridTest, DefaultGridSize) {

	RobotWorldSimulator::RobotGrid grid;

	EXPECT_EQ(grid.getGridSize().height, RobotWorldSimulator::DEFAULT_HEIGHT);
	EXPECT_EQ(grid.getGridSize().width, RobotWorldSimulator::DEFAULT_WIDTH);

	EXPECT_TRUE(grid.isOffTheGrid({ 11, 11, "NORTH"}));
	EXPECT_FALSE(grid.isOffTheGrid({ 10, 10, "SOUTH" }));
}

TEST(RobotGridTest, CustomGridSize) {

	RobotWorldSimulator::RobotGrid grid{ 5,5 };

	EXPECT_EQ(grid.getGridSize().height, 5);
	EXPECT_EQ(grid.getGridSize().width, 5);

	EXPECT_TRUE(grid.isOffTheGrid({ 11, 11, "NORTH" }));
	EXPECT_FALSE(grid.isOffTheGrid({ 5, 5, "SOUTH" }));
}

TEST(RobotGridTest, ResizeGrid) {
	
	// Initialize to 5 X 5
	RobotWorldSimulator::RobotGrid grid{ 5,5 };

	EXPECT_EQ(grid.getGridSize().height, 5);
	EXPECT_EQ(grid.getGridSize().width, 5);

	EXPECT_TRUE(grid.isOffTheGrid({ 10, 10, "NORTH" }));

	grid.resizeGrid(10, 10);

	EXPECT_FALSE(grid.isOffTheGrid({ 10, 10, "NORTH" }));
}

TEST(RobotGridTest, PullRobotFromTheGrid)
{
	RobotWorldSimulator::RobotGrid grid;

	RobotFactory::RobotLocation location{ 0, 0, "NORTH" };
	const std::string name{ "NEO" };

	const auto robot = std::make_shared<RobotFactory::Marvin>(location, name);
	
	grid.addRobot(robot);

	const auto neo = grid.getRobot(location);
	EXPECT_EQ(name, neo->name());
}

TEST(RobotGridTest, MoveRobotToADifferentLocation)
{
	RobotWorldSimulator::RobotGrid grid;

	RobotFactory::RobotLocation location{ 0, 0, "NORTH" };
	const std::string name{ "NEO" };

	const auto robot = std::make_shared<RobotFactory::Marvin>(location, name);

	grid.addRobot(robot);

	auto neo = grid.getRobot(location);
	EXPECT_EQ(name, neo->name());

	const auto prev_location = location;

	RobotFactory::RobotLocation matrix2{ 3, 3, "SOUTH" };
	neo->setLocation(matrix2);

	grid.updateLocation(prev_location, neo);

	const auto neo2 = grid.getRobot(matrix2);
	const auto new_location = neo2->location();
	EXPECT_EQ(matrix2.x_coordinate, new_location.x_coordinate);
	EXPECT_EQ(matrix2.y_coordinate, new_location.y_coordinate);
	EXPECT_EQ(matrix2.direction, new_location.direction);
}