#pragma once

#include <deque>
#include <functional>
#include <optional>

#include "Command.h"
#include "Constants.h"

namespace hex 
{
	class Command;
	class CommandManager
	{
		std::deque<Command> m_commandQueue;

	public:

		bool isQEmpty() const;
		void pushCommand(const Command& pCommand);
		std::optional<Command> popCommand();

		virtual void onCommandExecutionEnds(CmdKind) = 0;
		virtual void onCommandExecutionBegins(CmdKind) = 0;
	};
}


namespace hex
{
	inline bool CommandManager::isQEmpty() const {
		return m_commandQueue.empty();
	}

	inline void CommandManager::pushCommand(const Command& pCommand) {
		m_commandQueue.push_back(pCommand);
	}

	inline std::optional<Command> CommandManager::popCommand()
	{
		if (!m_commandQueue.empty()) {
			Command command = m_commandQueue.front();
			m_commandQueue.pop_front();
			return std::make_optional(command);
		}
		return std::nullopt;
	}
}