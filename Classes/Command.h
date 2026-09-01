#pragma once

#include "Constants.h"
#include "CommandManager.h"

namespace hex
{
	class CommandManager;

	class Command
	{
		const CmdKind m_cmdKind;
		const ExecutionKind m_exeKind;

	public:

		~Command();
		Command(CmdKind pCmdType, ExecutionKind pExeType);

		GET(CmdKind, CommandKind, m_cmdKind)
		GET(ExecutionKind, ExecutionKind, m_exeKind)

		//virtual void execute() = 0;
		//virtual void finishedExecution();
	};
}


namespace hex
{
	inline Command::Command(CmdKind pCmdKind, ExecutionKind pExeKind)
		: m_cmdKind(pCmdKind)
		, m_exeKind(pExeKind)
	{ }
}