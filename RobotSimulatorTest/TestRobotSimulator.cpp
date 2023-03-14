#include "pch.h"
#include "RobotSimulator.h"

TEST(TestRobotSimulator, SimulateTheToyRobotApp)
{
	RobotWorldSimulator::RobotGrid matrix;
	RobotWorldSimulator::RobotSimulator simulate{ matrix };

	RobotFactory::RobotLocation location{ 0, 0, "NORTH" };
	
	simulate.place(location, "Neo");
	const auto robot = simulate.getRobot(location);
	EXPECT_EQ("Neo", robot->name());
	
	EXPECT_NO_FATAL_FAILURE(simulate.report());
}