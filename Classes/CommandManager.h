#pragma once

#include <deque>
#include <functional>

#include "Constants.h"

namespace hex 
{
	class Command;

	class CommandManager
	{
		friend Command;

		std::deque<Command*> m_commandQueue;

		CmdKind m_lastExecutedCommandType;

	public:

		CmdKind getLastExecutedCommandId();

		bool isQEmpty() const;
		void pushCommand(Command* pCommand);
		Command* popCommand();

		virtual void onCommandExecutionEnds(CmdKind) = 0;
		virtual void onCommandExecutionBegins(CmdKind) = 0;
	};
}