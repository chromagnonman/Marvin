#include "pch.h"
#include "RobotSimulator.h"

using namespace Simulator;
using namespace RobotFactory;

TEST(SimulateRobot, CreationAndMovement)
{
    RobotSimulator simulate;

    RobotLocation location{ 0, 0, "NORTH" };

    ROBOT_TYPE R2D2 {RobotType::Ground_based::Bipedaled};
    constexpr auto target_robot{"R2D2"};

    // Place robot into the grid
    ASSERT_TRUE(simulate.place(R2D2, location, target_robot));
    
    // Don't allow robot on the same location
    ROBOT_TYPE Clone {RobotType::Ground_based::Bipedaled};
    ASSERT_FALSE(simulate.place(Clone, location, "Trooper"));

    EXPECT_TRUE(simulate.move(target_robot));
    EXPECT_TRUE(simulate.rotate(target_robot, "RIGHT"));
    EXPECT_TRUE(simulate.rotate(target_robot, "LEFT"));

    simulate.rotate(target_robot, "LEFT");  // Facing East (0,1)

    // R2D2 is at the edge and should NOT be able to move
    EXPECT_FALSE(simulate.move(target_robot));

    EXPECT_TRUE(simulate.remove(target_robot));
    
    // Grid is now empty; expect failures
    EXPECT_FALSE(simulate.move(target_robot));
    EXPECT_FALSE(simulate.rotate(target_robot, "LEFT"));
    EXPECT_FALSE(simulate.rotate(target_robot, "RIGHT"));
    EXPECT_FALSE(simulate.remove(target_robot));
}
