#include "InputHandler.h"

namespace Utils {

	struct InputHandler::impl {

		impl(std::string&& input) noexcept;

		std::string parseCommand() noexcept;
		bool isValidInput() const noexcept;

	private:
		std::string m_input;
	};

	InputHandler::impl::impl(std::string&& input) noexcept : m_input{ input }
	{
	}

	bool InputHandler::impl::isValidInput() const noexcept
	{
		return true;
	}

	std::string InputHandler::impl::parseCommand() noexcept
	{
		std::string command{};

		return std::move(command);
	}

	InputHandler::InputHandler(std::string input) noexcept : m_pImpl { std::make_unique<impl>(std::move(input))}
	{
	}

	bool InputHandler::isValidInput() const noexcept
	{
		return m_pImpl->isValidInput();
	}

	std::string InputHandler::parseCommand() noexcept
	{
		return m_pImpl->parseCommand();
	}

}