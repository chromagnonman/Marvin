#include "pch.h"
#include "RobotSimulator.h"

using namespace Simulator;

TEST(SimulateRobot, CreationAndMovement)
{
	RobotSimulator simulate;

	RobotFactory::RobotLocation location{ 0, 0, "NORTH" };
    RobotFactory::Marvin R2D2 {location, {"Neo"}};

    // Place robot onto the grid
	ASSERT_TRUE(simulate.place(R2D2));

    // Don't allow robot on the same location
    ASSERT_FALSE(simulate.place(R2D2));

	EXPECT_TRUE(simulate.move(R2D2.model()));
    EXPECT_TRUE(simulate.rotate(R2D2.model(), "RIGHT"));
    EXPECT_TRUE(simulate.rotate(R2D2.model(), "LEFT"));

    simulate.rotate(R2D2.model(), "LEFT");  // Facing East (0,1)

    // Neo is at the edge and should NOT be able to move
    EXPECT_FALSE(simulate.move(R2D2.model()));

    EXPECT_TRUE(simulate.remove(R2D2.model()));
    
    // Grid is now empty; expect failures
    EXPECT_FALSE(simulate.move(R2D2.model()));
    EXPECT_FALSE(simulate.rotate(R2D2.model(), "LEFT"));
    EXPECT_FALSE(simulate.rotate(R2D2.model(), "RIGHT"));
    EXPECT_FALSE(simulate.remove(R2D2.model()));
}
