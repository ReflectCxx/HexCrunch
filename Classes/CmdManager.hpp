
#include "CmdManager.h"

namespace hex
{
	inline void CmdManager::blockQ(bool block) {
		m_qBlocked = block;
	}


	inline void CmdManager::pushCommand(const Command& pCommand) {
		m_commandQueue.push_back(pCommand);
	}


	inline std::optional<Command> CmdManager::popCommand()
	{
		if (m_commandQueue.empty() || m_qBlocked) {
			return std::nullopt;
		}

		Command command = m_commandQueue.front();
		m_commandQueue.pop_front();
		return std::make_optional(command);
	}
}