#pragma once

#include "cocos2d.h"

#include "Command.h"
#include "Game.h"

namespace hex
{
	class HexTile;
	struct CmdSwapTile
	{
		static Command create(HexTile& pActor, HexTile& pFollower)
		{
			return Command{

				BlocksQ::Yes,
				CmdKind::SwapTiles,
				Game::instance().fxController(),
				[action = CmdSwapTile{ pActor, pFollower }]
				(Command& pCmd) mutable {
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