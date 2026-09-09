#pragma once

#include <functional>
#include "Constants.h"

namespace hex
{
	class CommandController;

	class Command final
	{
		friend CommandController;

		inline static std::uint32_t m_counter{ 0 };

		const BlocksQ m_blockQ;
		const CmdKind m_cmdKind;
		const std::size_t m_cmdId;
		std::function<void(Command&)> m_command;

		CmdState m_cmdState;
		CommandController& m_controller;

		Command(const Command&) = delete;
		Command& operator=(Command&&) = delete;
		Command& operator=(const Command&) = delete;

	public:
		
		void end();
		void execute();
		void unblockQ();
		constexpr CmdKind getKind() const;

		Command(Command&&) noexcept;
		Command(BlocksQ pBlockQ, const CmdKind pCmdK,
				CommandController& pCC, std::function<void(Command&)> pCmd);
	};
}

#include "Command.hpp"
#include "CommandController.hpp"