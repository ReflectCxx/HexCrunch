#pragma once

#include <deque>
#include <optional>

namespace hex
{
	class Command;

	class CommandController
	{
		friend Command;

		bool m_qBlocked = false;
		std::size_t m_runningCount = 0;
		std::uint64_t m_blockedByCmdId = -1;
		std::deque<Command> m_commands;
		std::deque<std::reference_wrapper<Command>> m_commandQ;

		void blockQ(const std::uint64_t pByCmdId, const bool);
		constexpr std::size_t& runningCount();

	public:

		void update();
		constexpr std::size_t getRunningCmdCount() const;
		std::optional<std::reference_wrapper<Command>> nextCmd();

	protected:

		void push(Command);

		CommandController() = default;
		CommandController(CommandController&&) = delete;
		CommandController(const CommandController&) = delete;
		CommandController& operator=(CommandController&&) = delete;
		CommandController& operator=(const CommandController&) = delete;
	};
}