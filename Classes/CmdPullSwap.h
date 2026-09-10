#pragma once

#include "Game.h"
#include "HexTile.h"
#include "Command.h"
#include "GridFxController.h"

namespace hex
{
	struct CmdPullSwap
	{
		static std::optional<Command> create(HexTile& pTile)
		{
			auto action = CmdPullSwap{ pTile, nullptr };
			if (action.init()) {
				return Command{

					BlocksQ::No,
					CmdKind::PullSwap,
					Game::instance().fxController(),
					[action] (Command& pCmd)-> void {
						action.run(pCmd);
					}
				};
			}
			return std::nullopt;
		}

		HexTile& m_tile;
		HexTile* m_pullTile;

		void run(Command& pCmd) const;

	private:

		bool init();
	};
}