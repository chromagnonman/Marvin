#include "Command.h"
#include "Robot.h"

#include <gtest/gtest.h>

#include <string>

namespace
{

TEST(CommandParser, ParsesPlacement)
{
    const auto result = Simulator::CommandParser::parse("place R2D2 2,3 west");

    ASSERT_TRUE(result);
    const auto &command = std::get<Simulator::PlaceCommand>(*result.command);
    EXPECT_EQ(command.name, "R2D2");
    EXPECT_EQ(command.location.x, 2);
    EXPECT_EQ(command.location.y, 3);
    EXPECT_EQ(command.location.direction, RobotFactory::Direction::West);
}

TEST(CommandParser, ParsesIdTarget)
{
    const auto result = Simulator::CommandParser::parse("MOVE @43 4");

    ASSERT_TRUE(result);
    const auto &command = std::get<Simulator::MoveCommand>(*result.command);
    ASSERT_TRUE(command.target.has_value());
    const auto target = command.target.value_or(Simulator::RobotTarget{std::string{}});
    EXPECT_EQ(std::get<RobotFactory::RobotId>(target.value), 43U);
    EXPECT_EQ(command.blocks, 4U);
}

TEST(CommandParser, RejectsMalformedNumbers)
{
    EXPECT_FALSE(Simulator::CommandParser::parse("MOVE R2D2 nope"));
    EXPECT_FALSE(Simulator::CommandParser::parse("PLACE R2D2 -1,0 NORTH"));
    EXPECT_FALSE(Simulator::CommandParser::parse("RESIZE 0 10"));
}

TEST(CommandParser, RejectsUnknownCommandsAndExtraArguments)
{
    EXPECT_FALSE(Simulator::CommandParser::parse("FLY R2D2"));
    EXPECT_FALSE(Simulator::CommandParser::parse("REPORT extra"));
}

} // namespace
