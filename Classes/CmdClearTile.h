#pragma once

#include "Command.h"
#include "HexTile.h"
#include "Game.h"

namespace hex
{
	struct CmdClearTile
	{
		static Command create(HexTile& pTile)
		{
			return Command{

				BlocksQ::Yes,
				CmdKind::ClearTile,
				Game::instance().fxController(),
				[action = CmdClearTile{ pTile }]
				(Command& pCmd)->void {
					action.run(pCmd);
				}
			};
		}

		HexTile& m_tile;
		void run(Command& pCmd) const;
	};


	void CmdClearTile::run(Command& pCmd) const
	{
		constexpr auto DT = 0.25f;		
		const auto scaleUp = cocos2d::ScaleTo::create(DT * 1.f/ 4.f, 1.15f);
		const auto scaleDown = cocos2d::ScaleTo::create(DT * 3.f/ 4.f, 0.01f);

		m_tile.getForeground().runAction(
			cocos2d::Sequence::create( scaleUp,
				cocos2d::CallFunc::create([&]()->void {
					pCmd.unblockQ();
				}), scaleDown,
				cocos2d::CallFunc::create([&]()->void {
					m_tile.getForeground().setScale(1.f);
					m_tile.getForeground().setVisible(false);
					pCmd.end();
				}), nullptr
			)
		);
		m_tile.setState(TileState::Idle);
		m_tile.getLink().setVisible(true);
	}
}