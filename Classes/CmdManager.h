#pragma once

#include <deque>
#include <optional>

#include "Command.h"
#include "Constants.h"

namespace hex 
{
	class Command;
	class CmdManager
	{
		friend Command;

		bool m_qBlocked;
		
		std::deque<Command> m_commandQueue;

		void blockQ(bool);

	public:

		std::optional<Command> popCommand();
		void pushCommand(const Command& pCommand);
	};
}