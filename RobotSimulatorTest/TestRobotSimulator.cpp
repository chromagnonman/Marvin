#include "pch.h"
#include "RobotSimulator.h"

using namespace Simulator;
using namespace RobotFactory;

TEST(SimulateRobot, CreationAndMovement)
{
	RobotSimulator simulate;

	RobotLocation location{ 0, 0, "NORTH" };
    constexpr auto target_robot{"R2D2"};

    std::unique_ptr<Robot> R2D2 =
        std::make_unique<Marvin>(location, target_robot);

    // Place robot onto the grid
	ASSERT_TRUE(simulate.place(std::move(R2D2)));

    std::unique_ptr<Robot> clone =
        std::make_unique<Marvin>(location, "trooper");

    // Don't allow robot on the same location
    ASSERT_FALSE(simulate.place(std::move(clone)));

	EXPECT_TRUE(simulate.move(target_robot));
    EXPECT_TRUE(simulate.rotate(target_robot, "RIGHT"));
    EXPECT_TRUE(simulate.rotate(target_robot, "LEFT"));

    simulate.rotate(target_robot, "LEFT");  // Facing East (0,1)

    // Neo is at the edge and should NOT be able to move
    EXPECT_FALSE(simulate.move(target_robot));

    EXPECT_TRUE(simulate.remove(target_robot));
    
    // Grid is now empty; expect failures
    EXPECT_FALSE(simulate.move(target_robot));
    EXPECT_FALSE(simulate.rotate(target_robot, "LEFT"));
    EXPECT_FALSE(simulate.rotate(target_robot, "RIGHT"));
    EXPECT_FALSE(simulate.remove(target_robot));
}
