#pragma once

#include <memory>
#include <string>
#include <vector>

namespace RobotCommands {

	enum class ROBOT_COMMAND : size_t {
		PLACE = 0,
		MOVE,
		LEFT,
		RIGHT,
		REPORT
	};
	/**
	* @brief
	*/
	class RobotCommandHandler {
	public:
		explicit RobotCommandHandler(std::string input) noexcept;
		virtual ~RobotCommandHandler() noexcept;
		std::vector<std::string> getCommand() const noexcept;

	private:
		struct impl;
		std::unique_ptr<impl> m_pImpl;
	};
}