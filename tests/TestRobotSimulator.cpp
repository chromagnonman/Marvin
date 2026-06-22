#include "marvin/robot/Robot.h"
#include "marvin/robot/RobotAssembly.h"
#include "marvin/simulator/RobotSimulator.h"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

namespace
{

TEST(RobotSimulator, EnforcesUniqueCaseInsensitiveNames)
{
    Simulator::RobotSimulator simulator;
    const RobotFactory::RobotLocation first{
        .x = 0, .y = 0, .direction = RobotFactory::Direction::North};
    const RobotFactory::RobotLocation second{
        .x = 1, .y = 0, .direction = RobotFactory::Direction::North};

    EXPECT_TRUE(simulator.place(RobotFactory::GroundRobotType::Bipedal, first, "R2D2"));
    EXPECT_FALSE(simulator.place(RobotFactory::GroundRobotType::Bipedal, second, "r2d2"));
    EXPECT_EQ(simulator.robotCount(), 1U);
}

TEST(RobotSimulator, SupportsIdTargeting)
{
    Simulator::RobotSimulator simulator;
    ASSERT_TRUE(simulator.place(RobotFactory::GroundRobotType::Bipedal,
                                {.x = 0, .y = 0, .direction = RobotFactory::Direction::North},
                                "R2D2"));
    const auto *robot = simulator.findRobot("r2d2");
    ASSERT_NE(robot, nullptr);
    const auto id = robot->id();

    EXPECT_TRUE(simulator.move(id, 2));
    ASSERT_NE(simulator.findRobot(id), nullptr);
    EXPECT_EQ(simulator.findRobot(id)->location().y, 2);
    EXPECT_TRUE(simulator.remove(id));
    EXPECT_EQ(simulator.findRobot(id), nullptr);
}

TEST(RobotSimulator, PreventsCollisionsDuringMoveAll)
{
    Simulator::RobotSimulator simulator{{.width = 3, .height = 3}};
    ASSERT_TRUE(simulator.place(RobotFactory::GroundRobotType::Bipedal,
                                {.x = 1, .y = 0, .direction = RobotFactory::Direction::North},
                                "NORTHBOUND"));
    ASSERT_TRUE(simulator.place(RobotFactory::GroundRobotType::Bipedal,
                                {.x = 1, .y = 2, .direction = RobotFactory::Direction::South},
                                "SOUTHBOUND"));

    EXPECT_EQ(simulator.moveAll(), 1U);
    const auto first = simulator.findRobot("NORTHBOUND")->location();
    const auto second = simulator.findRobot("SOUTHBOUND")->location();
    EXPECT_NE(first.y, second.y);
}

TEST(RobotSimulator, RejectsShrinkingWithRobotsPresent)
{
    Simulator::RobotSimulator simulator{{.width = 5, .height = 5}};
    ASSERT_TRUE(simulator.place(RobotFactory::GroundRobotType::Bipedal,
                                {.x = 4, .y = 4, .direction = RobotFactory::Direction::North},
                                "EDGE"));

    EXPECT_FALSE(simulator.resize({.width = 4, .height = 4}));
    EXPECT_EQ(simulator.gridSize().width, 5);
}

TEST(RobotSimulator, ReportsMalformedInputWithoutThrowing)
{
    Simulator::RobotSimulator simulator;
    std::ostringstream output;
    std::ostringstream errors;

    EXPECT_TRUE(simulator.executeLine("MOVE R2D2 bananas", output, errors));
    EXPECT_NE(errors.str().find("positive integer"), std::string::npos);
}

TEST(RobotSimulator, RunsCommandLoopUntilQuit)
{
    Simulator::RobotSimulator simulator;
    std::istringstream input{"PLACE R2D2 0,0 NORTH\nMOVE R2D2 2\nREPORT\nQUIT\n"};
    std::ostringstream output;
    std::ostringstream errors;

    simulator.run(input, output, errors);

    ASSERT_NE(simulator.findRobot("R2D2"), nullptr);
    EXPECT_EQ(simulator.findRobot("R2D2")->location().y, 2);
    EXPECT_TRUE(errors.str().empty());
    EXPECT_NE(output.str().find("Robots: 1"), std::string::npos);
}

} // namespace
