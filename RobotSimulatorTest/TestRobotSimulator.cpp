#include "pch.h"
#include "RobotSimulator.h"

using namespace Simulator;

TEST(SimulateRobot, CreationAndMovement)
{
	RobotGrid matrix;
	RobotSimulator simulate{ matrix };

	RobotFactory::RobotLocation location{ 0, 0, "NORTH" };
    RobotParameters Neo {location, {}, {"Neo"}};

    // Place robot onto the grid
	ASSERT_TRUE(simulate.place(Neo));

	EXPECT_TRUE(simulate.move(Neo));
    EXPECT_TRUE(simulate.rotateRight(Neo));
    EXPECT_TRUE(simulate.rotateLeft(Neo));

    simulate.rotateLeft(Neo); // Facing East (0,1)

    // Neo is at the edge and should NOT be able to move
    EXPECT_FALSE(simulate.move(Neo));

    EXPECT_TRUE(simulate.remove(Neo));

    // Grid is now empty; expect failures
    EXPECT_FALSE(simulate.move(Neo));
    EXPECT_FALSE(simulate.rotateLeft(Neo));
    EXPECT_FALSE(simulate.rotateRight(Neo));
}
