#pragma once

#include "Command.h"

namespace hex
{
	constexpr CmdKind Command::getKind() const {
		return m_cmdKind;
	}

	inline bool Command::execute() {
		return m_command(*this);
	}


	inline void Command::executionEnds()
	{
		if (m_blocksCmdQ) {
			m_controller.blockQ(false);
		}
		m_controller.runningCount()--;
		m_controller.pop();
	}


	inline void Command::executionBegins()
	{
		if (m_blocksCmdQ) {
			m_controller.blockQ(true);
		}
		m_controller.runningCount()++;
	}
}


namespace hex
{
	constexpr CmdKind CommandController::getRunningCmd() const {
		if (m_runningCount != 0) {
			return m_commandQ.front().getKind();
		}
		return CmdKind::None;
	}

	constexpr std::size_t& CommandController::runningCount() {
		return m_runningCount;
	}

	inline void CommandController::blockQ(bool pBlock) {
		m_qBlocked = pBlock;
	}

	inline void CommandController::pop() {
		m_commandQ.pop_front();
	}

	inline void CommandController::push(const Command& pCmd) {
		m_commandQ.push_back(pCmd);
	}

	inline std::optional<std::reference_wrapper<Command>> CommandController::nextCmd()
	{
		if (m_commandQ.empty() || m_qBlocked) {
			return std::nullopt;
		}
		return { std::reference_wrapper<Command>(m_commandQ.front()) };
	}
}