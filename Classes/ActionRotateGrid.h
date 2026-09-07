#pragma once

#include "Command.h"
#include "HexGrid.h"

namespace hex
{
	struct ActionRotateGrid
	{
		const float m_angle;
		HexGrid& m_grid;

		bool run(Command& pCmd) const;

		static Command create(CommandController& pController, HexGrid& pGrid, const float pAngle)
		{
			const auto action = ActionRotateGrid{ pAngle, pGrid };
			return {
				.m_blocksCmdQ = true,
				.m_cmdKind = CmdKind::RotateGrid,
				.m_controller = pController,
				.m_command = [action](Command& pCmd) {
					return action.run(pCmd);
				}
			};
		}
	};
}



namespace hex
{
	bool ActionRotateGrid::run(Command& pCmd) const
	{
		pCmd.executionBegins();

		m_grid.runAction(
			cocos2d::Sequence::create(
				cocos2d::RotateBy::create(0.5f, m_angle),
				cocos2d::CallFunc::create([&]() {
					pCmd.executionEnds();
				}),
				nullptr));
		return true;
	}
}