#pragma once

#include "cocos2d.h"

#include "Command.h"
#include "Game.h"

namespace hex
{
	class HexTile;
	struct CmdSliderSwap
	{
		static Command create(HexTile& pActor, HexTile& pFollower)
		{
			return Command{

				BlocksQ::Yes,
				Game::instance().fxController(),
				[action = CmdSliderSwap{ pActor, pFollower }]
				(Command& pCmd) mutable-> void{
					action.run(pCmd); 
				} 
			};
		}

		void run(Command& pCmd);

		HexTile& m_actor;
		HexTile& m_follower;

	private:

		void setZOrder(int pZOdr);
		void onEnd(Command& pCmd, int pZOdr, const cocos2d::Vec2& pActorPos,
				   const cocos2d::Vec2& pOtherPos);
	};
}