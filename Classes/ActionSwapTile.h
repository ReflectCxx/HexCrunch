#pragma once

#include "Command.h"

namespace hex
{
	class HexTile;
	struct ActionSwapTile
	{
		HexTile& m_tileA;
		HexTile& m_tileB;

		bool run(Command& pCmd) const;

		static Command create(CommandController& pController, HexTile& pTileA, HexTile& pTileB)
		{
			const auto action = ActionSwapTile{ pTileA, pTileB };
			return {
				.m_blocksCmdQ = true,
				.m_cmdKind = CmdKind::SwapTiles,
				.m_controller = pController,
				.m_command = [action](Command& pCmd) { return action.run(pCmd); }
			};
		}
	};
}