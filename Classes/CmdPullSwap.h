#pragma once

#include "Game.h"
#include "HexTile.h"
#include "Command.h"
#include "GridFxController.h"

namespace hex
{
	struct CmdPullSwap
	{
		static Command create(HexTile& pTile, HexTile& pPullFrom)
		{
			return Command{

				BlocksQ::No,
				Game::instance().fxController(),
				[action = CmdPullSwap{ pTile, pPullFrom }]
				(Command& pCmd)-> void {
					action.run(pCmd);
				}
			};
		}

		void run(Command& pCmd) const;

	private:

		HexTile& m_tile;
		const cocos2d::Vec2 m_pullFromPos;

		CmdPullSwap(HexTile& pTile, HexTile& pPullFrom);
	};
}