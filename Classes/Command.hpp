#pragma once

#include "cocos2d.h"

#include "Command.h"

namespace hex
{
	constexpr CmdKind Command::getKind() const {
		return m_cmdKind;
	}


	inline Command::Command(bool pBlocksQ, const CmdKind pCmdK ,
						    CommandController& pCC, std::function<void(Command&)> pCmd)
		: m_blocksCmdQ(pBlocksQ)
		, m_cmdKind(pCmdK)
		, m_command(std::move(pCmd))
		, m_cmdState(CmdState::None)
		, m_controller(pCC)
		, m_cmdId(m_counter++){
	}


	inline Command::Command(Command&& pOther) noexcept
		: m_blocksCmdQ(pOther.m_blocksCmdQ)
		, m_cmdKind(pOther.m_cmdKind)
		, m_command(std::move(pOther.m_command))
		, m_controller(pOther.m_controller)
		, m_cmdState(pOther.m_cmdState)
		, m_cmdId(pOther.m_cmdId) {
		pOther.m_cmdState = CmdState::Expired;
	}


	inline void Command::execute()
	{
		if (m_cmdState != CmdState::Ready) {
			return;
		}
		m_cmdState = CmdState::Running;
		m_controller.runningCount()++;
		if (m_blocksCmdQ) {
			m_controller.blockQ(m_cmdId, true);
		}
		CCASSERT(m_command, "Command callback cannot be empty");
		m_command(*this);
	}

	
	inline void Command::end()
	{
		if (m_cmdState != CmdState::Running) {
			return;
		}
		if (m_blocksCmdQ) {
			m_controller.blockQ(m_cmdId, false);
		}
		m_controller.runningCount()--;
		m_cmdState = CmdState::Expired;
	}
}



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