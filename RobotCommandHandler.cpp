#include "RobotCommandHandler.h"

namespace RobotCommands {

	struct RobotCommandHandler::impl {
		impl(std::string input) noexcept;
		std::vector<std::string> getCommand() const noexcept;
		void parseInput(const std::string& input) noexcept;

	private:
		std::vector<std::string> m_commands;
		std::string m_inputCommand;
	};

	RobotCommandHandler::impl::impl(std::string input) noexcept : m_inputCommand{ std::move(input) }
	{
	}

	std::vector<std::string> RobotCommandHandler::impl::getCommand() const noexcept
	{

		return m_commands;
	}

	void RobotCommandHandler::impl::parseInput(const std::string& input) noexcept
	{

	}
	
	RobotCommandHandler::RobotCommandHandler(std::string input) noexcept :
	m_pImpl{std::make_unique<impl>(input)}
	{
	}

	RobotCommandHandler::~RobotCommandHandler() = default;

	std::vector<std::string> RobotCommandHandler::getCommand() const noexcept
	{
		return m_pImpl->getCommand();
	}


}