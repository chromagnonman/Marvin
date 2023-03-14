#include "pch.h"
#include "RobotGrid.h"

TEST(RobotGridTest, TestDefaultGridSize) {

	RobotWorldSimulator::RobotGrid grid;
	EXPECT_EQ(grid.getGridSize().height, RobotWorldSimulator::DEFAULT_HEIGHT);
	EXPECT_EQ(grid.getGridSize().width, RobotWorldSimulator::DEFAULT_WIDTH);
}