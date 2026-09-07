#pragma once

#include "Command.h"
#include "HexGrid.h"

namespace hex
{
	struct ActionRotateGrid
	{
		const float m_angle;
		HexGrid& m_grid;

		void run(Command& pCmd) const;

		static Command create(CommandController& pController, HexGrid& pGrid, const float pAngle)
		{
			const auto action = ActionRotateGrid{ pAngle, pGrid };
			return Command {
				true,
				pController,
				CmdKind::RotateGrid,
				[action](Command& pCmd) { 
					action.run(pCmd); 
				} 
			};
		}
	};
}



namespace hex
{
	void ActionRotateGrid::run(Command& pCmd) const
	{
		m_grid.runAction(
			cocos2d::Sequence::create(
				cocos2d::RotateBy::create(0.5f, m_angle), 
				cocos2d::CallFunc::create([&]() {
					pCmd.end();
				}),
				nullptr)
		);
	}
}