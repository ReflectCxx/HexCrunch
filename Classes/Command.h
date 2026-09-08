#pragma once

#include <deque>
#include <optional>
#include <functional>

#include "Constants.h"


namespace hex
{
	class CommandController;

	class Command final
	{
		friend CommandController;

		inline static std::uint32_t m_counter{ 0 };

		const BlocksQ m_blockQ;
		const CmdKind m_cmdKind;
		const std::size_t m_cmdId;
		std::function<void(Command&)> m_command;

		CmdState m_cmdState;
		CommandController& m_controller;

		Command(const Command&) = delete;
		Command& operator=(Command&&) = delete;
		Command& operator=(const Command&) = delete;

	public:
		
		void end();
		void execute();
		constexpr CmdKind getKind() const;

		Command(Command&&) noexcept;
		Command(BlocksQ pBlockQ, const CmdKind pCmdK,
				CommandController& pCC, std::function<void(Command&)> pCmd);
	};
}



namespace hex
{
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


#include "Command.hpp"