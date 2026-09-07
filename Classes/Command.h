#pragma once

#include <deque>
#include <optional>
#include <functional>

#include "Constants.h"

namespace hex
{
	class CommandController;

	struct Command final
	{
		const bool m_blocksCmdQ;
		const CmdKind m_cmdKind;
		CommandController& m_controller;
		const std::function<bool(Command&)> m_command;

		bool execute();
		void executionEnds();
		void executionBegins();
		constexpr CmdKind getKind() const;
	};
}


namespace hex
{
	class CommandController
	{
		friend Command;

		bool m_qBlocked = false;
		std::size_t m_runningCount = 0;
		CmdKind m_runningCmd = CmdKind::None;

		std::deque<Command> m_commandQ;

		void pop();
		void blockQ(bool);
		constexpr std::size_t& runningCount();

	public:

		constexpr CmdKind getRunningCmd() const;

		std::optional<std::reference_wrapper<Command>> nextCmd();

	protected:

		void push(const Command&);
	};
}


#include "Command.hpp"