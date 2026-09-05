#pragma once

#include <functional>

#include "Constants.h"
#include "CommandManager.h"

namespace hex
{
	class CommandManager;

	class Command
	{
		const CmdKind m_cmdKind;
		const ExecutionKind m_exeKind;
		std::function<bool()> m_command;

	public:

		~Command();
		Command(CmdKind pCmdType, ExecutionKind pExeType);

		GET(CmdKind, CommandKind, m_cmdKind)
		GET(ExecutionKind, ExecutionKind, m_exeKind)

		bool execute();
		//virtual void finishedExecution();
	};
}


namespace hex
{
	inline Command::Command(CmdKind pCmdKind, ExecutionKind pExeKind)
		: m_cmdKind(pCmdKind)
		, m_exeKind(pExeKind)
	{ }

	inline bool Command::execute() {
		return m_command();
	}
}