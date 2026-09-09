#pragma once

#include "cocos2d.h"
#include "CommandController.h"

namespace hex
{
	constexpr std::size_t CommandController::getRunningCmdCount() const {
		return m_runningCount;
	}


	constexpr std::size_t& CommandController::runningCount() {
		return m_runningCount;
	}


	inline void CommandController::update()
	{
		int count = 0;
		while (!m_commands.empty() && m_commands.front().m_cmdState == CmdState::Expired) {
			m_commands.pop_front();
			count++;
		}
		if (count > 0) {
			CCLOG("Expired cmds count: %d", count);
		}
	}


	inline void CommandController::push(Command pCmd)
	{
		if (pCmd.m_cmdState != CmdState::None) {
			return;
		}
		m_commands.push_back(std::move(pCmd));
		m_commandQ.push_back(m_commands.back());
		m_commands.back().m_cmdState = CmdState::Queued;
	}


	inline std::optional<std::reference_wrapper<Command>> CommandController::nextCmd()
	{
		if (m_qBlocked || m_commandQ.empty()) {
			return std::nullopt;
		}
		auto& cmd = m_commandQ.front().get();
		cmd.m_cmdState = CmdState::Ready;
		m_commandQ.pop_front();
		return cmd;
	}


	inline void CommandController::blockQ(const std::uint64_t pByCmdId, const bool pBlock)
	{
		if (pBlock) {
			if (!m_qBlocked) {
				m_qBlocked = true;
				m_blockedByCmdId = pByCmdId;
			}
		}
		else if (m_blockedByCmdId == pByCmdId) {
			m_qBlocked = false;
			m_blockedByCmdId = -1;
		}
	}
}