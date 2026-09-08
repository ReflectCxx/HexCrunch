#pragma once

#include "Command.h"
#include "HexGrid.h"
#include "Game.h"

namespace hex
{
	struct ActionRotateGrid
	{
		const float m_angle;

		void run(Command& pCmd) const;

		static Command create(const float pAngle)
		{
			const auto action = ActionRotateGrid{ pAngle };
			return Command {
				true,
				CmdKind::RotateGrid,
				Game::instance().fxController(),
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
		Game::instance().grid().runAction(
			cocos2d::Sequence::create(
				cocos2d::RotateBy::create(0.5f, m_angle), 
				cocos2d::CallFunc::create([&]() {
					pCmd.end();
				}), nullptr
			)
		);
	}
}