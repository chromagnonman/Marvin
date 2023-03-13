#include "InputHandler.h"

namespace Utils {

	InputHandler::InputHandler(std::string input) noexcept : m_input{std::move(input)}
	{
	}

	bool InputHandler::isValidInput() const noexcept
	{
		return true;
	}

	std::string InputHandler::parseCommand() noexcept
	{
		std::string command{};

		return std::move(command);
	}

}