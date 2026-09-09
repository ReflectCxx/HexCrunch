#pragma once

#include "cocos2d.h"

#include "Game.h"
#include "HexTile.h"
#include "Command.h"
#include "GridFxController.h"

namespace hex
{
	struct CmdSwapTiles
	{
		static Command create(HexTile& pTileA, HexTile& pTileB)
		{
			return Command{

				BlocksQ::No,
				CmdKind::SwapTiles,
				Game::instance().fxController(),
				[action = CmdSwapTiles{ pTileA, pTileB }]
				(Command& pCmd)-> void {
					action.run(pCmd);
				}
			};
		}

		void run(Command& pCmd) const;

		HexTile& m_tileA;
		HexTile& m_tileB;

	private:

		void onEnd(Command& pCmd, const cocos2d::Vec2& pPosA, const cocos2d::Vec2& pPosB) const;
	};
}