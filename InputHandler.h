#pragma once

#include <string>
#include <memory>
#include <unordered_set>

namespace Utils {
	
	class InputHandler {
	public:
		explicit InputHandler(std::string input) noexcept;
		virtual ~InputHandler() = default;

		[[nodiscard]] std::string parseCommand() noexcept;
		[[nodiscard]] bool isValidInput() const noexcept;

	private:
		std::string m_input;
	};
}