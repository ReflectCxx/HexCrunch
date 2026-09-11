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
			auto action = CmdPullSwap{ pTile };
			if (action.init()) {
				return Command{

					BlocksQ::No,
					Game::instance().fxController(),
					[action] (Command& pCmd)-> void {
						action.run(pCmd);
					}
				};
			}
			return std::nullopt;
		}

		HexTile& m_tile;
		cocos2d::Vec2 m_pullFromPos = { 0.f, 0.f };

		void run(Command& pCmd) const;

	private:

		bool init();
	};
}