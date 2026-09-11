#pragma once

#include "Command.h"
#include "HexGrid.h"
#include "Game.h"

namespace hex
{
	struct CmdRotateGrid
	{
		static Command create(const float pAngle)
		{
			return Command{

				BlocksQ::Yes,
				Game::instance().fxController(),
				[action = CmdRotateGrid{ pAngle }]
				(Command& pCmd)->void {
					action.run(pCmd); 
				} 
			};
		}

		const float m_angle;
		void run(Command& pCmd) const;
	};


	void CmdRotateGrid::run(Command& pCmd) const
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