#pragma once

#include <functional>

#include "Constants.h"
#include "CmdManager.h"

namespace hex
{
	class CmdManager;

	struct Command
	{
		const bool m_blocksCmdQ;
		const CmdKind m_cmdKind;
		const std::function<bool()> m_command;

		bool execute();

	protected:

		void executionEnds(CmdManager&);
		void executionBegins(CmdManager&);
	};
}