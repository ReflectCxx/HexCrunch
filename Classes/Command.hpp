#pragma once

#include "Command.h"
#include "CmdManager.h"

namespace hex
{
	inline bool Command::execute() {
		return m_command();
	}


	inline void Command::executionEnds(CmdManager& cmdMgr)
	{
		if (m_blocksCmdQ) {
			cmdMgr.blockQ(true);
		}
	}


	inline void hex::Command::executionBegins(CmdManager& cmdMgr)
	{
		if (m_blocksCmdQ) {
			cmdMgr.blockQ(false);
		}
	}
}