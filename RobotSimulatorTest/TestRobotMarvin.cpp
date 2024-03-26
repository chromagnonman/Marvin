#include "pch.h"
#include "Marvin.h"

TEST(TestRobotMarvin, RobotConstructionAndMovement)
{
	const RobotFactory::RobotLocation location{ 0, 0, "NORTH" };
	const auto marvin = std::make_shared<RobotFactory::Marvin>(location, "Marvin");

	// Attempt to move upwards where the robot is facing.
	marvin->move();

	// The Y coordinate should be greater than 0
	EXPECT_GT(marvin->location().y_coordinate, 0);

	// Rotate to the right facing east
	marvin->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);
	marvin->move();

	// The X coordinate should be greater than 0 now.
	EXPECT_GT(marvin->location().x_coordinate, 0);

	// Using abstract base class pointer to move Marvin up
	std::shared_ptr<RobotFactory::Robot> robot{ marvin };
	EXPECT_EQ(robot->name(), marvin->name());

	robot->rotate(); // Rotate left facing north
	robot->move();   // Move up one unit
	EXPECT_EQ(robot->location().y_coordinate, marvin->location().y_coordinate);
	EXPECT_EQ(robot->Id(), marvin->Id());
}
