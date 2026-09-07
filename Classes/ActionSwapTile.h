#pragma once

#include "Command.h"

namespace hex
{
	class HexTile;
	struct ActionSwapTile
	{
		HexTile& m_tileA;
		HexTile& m_tileB;

		void run(Command& pCmd) const;

		static Command create(CommandController& pController, HexTile& pTileA, HexTile& pTileB)
		{
			const auto action = ActionSwapTile{ pTileA, pTileB };
			return Command{ 
				pController, CmdKind::SwapTiles, true,
				[action](Command& pCmd) { 
					action.run(pCmd); 
				} 
			};
		}
	};
}