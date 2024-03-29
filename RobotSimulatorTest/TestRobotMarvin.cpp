#include "pch.h"
#include "Marvin.h"

TEST(Marvin, RobotCreationAndMovement)
{
	const RobotFactory::RobotLocation location{ 0, 0, "NORTH" };
	const auto marvin = std::make_shared<RobotFactory::Marvin>(location, "Marvin");

	marvin->move();

	EXPECT_GT(marvin->location().y_coordinate, 0);

	// Rotate to the right facing east
	marvin->rotate(RobotFactory::ROBOT_ROTATION::RIGHT);
	
	marvin->move();

	EXPECT_GT(marvin->location().x_coordinate, 0);

	std::shared_ptr<RobotFactory::Robot> robot{ marvin };
	EXPECT_EQ(robot->name(), marvin->name());

	robot->rotate(); // Rotate left facing north
	robot->move();   // Move up one unit
	EXPECT_EQ(robot->location().y_coordinate, marvin->location().y_coordinate);
	EXPECT_EQ(robot->Id(), marvin->Id());
}
