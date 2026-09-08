#pragma once

#include "Command.h"
#include "Game.h"

namespace hex
{
	class HexTile;
	struct ActionSwapTile
	{
		bool m_sliderSwap;
		HexTile& m_tileA;
		HexTile& m_tileB;

		void run(Command& pCmd) const;

		static Command create(HexTile& pTileA, HexTile& pTileB, bool pIsSliderSwap)
		{
			const auto action = ActionSwapTile{ pIsSliderSwap, pTileA, pTileB };
			return Command {
				true,
				CmdKind::SwapTiles,
				Game::instance().fxController(),
				[action](Command& pCmd) {
					action.run(pCmd); 
				} 
			};
		}
	};
}