#include "pch.h"
#include "RobotSimulator.h"

namespace TestSimulator {
	TEST(TestRobotSimulator, SimulateTheToyRobotApp)
	{
		RobotWorldSimulator::RobotGrid matrix;
		RobotWorldSimulator::RobotSimulator simulate{ matrix };

		RobotFactory::RobotLocation location{ 0, 0, "NORTH" };

		simulate.place(location, "Neo");
		const auto R2 = simulate.getRobot(location);
		EXPECT_EQ("Neo", R2->name());

		RobotFactory::RobotLocation new_location{ 4, 2, "NORTH" };
		
		simulate.place(new_location, "Buzz");
		const auto D2 = simulate.getRobot(new_location);
		EXPECT_EQ("Buzz", D2->name());

		simulate.move(); // Move upwards
		EXPECT_EQ(D2->location().y_coordinate, 3); // Y coordinate should have been incremented to 3

		// R2 and D2 should be facing left (West);
		simulate.rotateLeft();
		EXPECT_EQ(R2->location().direction, RobotFactory::ROBOT_DIRECTION::WEST);
		EXPECT_EQ(D2->location().direction, RobotFactory::ROBOT_DIRECTION::WEST);

		// R2 and D2 should be facing north again
		simulate.rotateRight();
		EXPECT_EQ(R2->location().direction, RobotFactory::ROBOT_DIRECTION::NORTH);
		EXPECT_EQ(D2->location().direction, RobotFactory::ROBOT_DIRECTION::NORTH);

		// R2 and D2 should be facing right (East)
		simulate.rotateRight();
		EXPECT_EQ(R2->location().direction, RobotFactory::ROBOT_DIRECTION::EAST);
		EXPECT_EQ(D2->location().direction, RobotFactory::ROBOT_DIRECTION::EAST);

		// R2 and D2 should be facing down (South)
		simulate.rotateRight();
		EXPECT_EQ(R2->location().direction, RobotFactory::ROBOT_DIRECTION::SOUTH);
		EXPECT_EQ(D2->location().direction, RobotFactory::ROBOT_DIRECTION::SOUTH);
	}
}