#pragma once

#include <string>
#include <memory>

namespace Utils {
	class InputHandler {
	public:
		explicit InputHandler(std::string input) noexcept;
		virtual ~InputHandler() = default;

		[[nodiscard]] std::string parseCommand() noexcept;
		[[nodiscard]] bool isValidInput() const noexcept;

	private:
		struct impl;
		std::unique_ptr<impl> m_pImpl;
	};
}