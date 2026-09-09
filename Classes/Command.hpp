#pragma once

#include "Command.h"
#include "CommandController.hpp"

namespace hex
{
	constexpr CmdKind Command::getKind() const {
		return m_cmdKind;
	}


	inline Command::Command(BlocksQ pBlockQ, const CmdKind pCmdK ,
						    CommandController& pCC, std::function<void(Command&)> pCmd)
		: m_blockQ(pBlockQ)
		, m_cmdKind(pCmdK)
		, m_command(std::move(pCmd))
		, m_cmdState(CmdState::None)
		, m_controller(pCC)
		, m_cmdId(m_counter++){
	}


	inline Command::Command(Command&& pOther) noexcept
		: m_blockQ(pOther.m_blockQ)
		, m_cmdKind(pOther.m_cmdKind)
		, m_command(std::move(pOther.m_command))
		, m_controller(pOther.m_controller)
		, m_cmdState(pOther.m_cmdState)
		, m_cmdId(pOther.m_cmdId) {
		pOther.m_cmdState = CmdState::Expired;
	}

	
	inline void Command::end()
	{
		unblockQ();
		m_controller.runningCount()--;
		m_cmdState = CmdState::Expired;
	}


	inline void Command::unblockQ()
	{
		if (m_cmdState != CmdState::Running) {
			return;
		}
		if (m_blockQ == BlocksQ::Yes) {
			m_controller.blockQ(m_cmdId, false);
		}
	}


	inline void Command::execute()
	{
		if (m_cmdState != CmdState::Ready) {
			return;
		}
		m_cmdState = CmdState::Running;
		m_controller.runningCount()++;
		if (m_blockQ == BlocksQ::Yes) {
			m_controller.blockQ(m_cmdId, true);
		}
		CCASSERT(m_command, "Command callback cannot be empty.");
		m_command(*this);
	}
}