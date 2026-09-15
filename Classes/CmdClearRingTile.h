#pragma once

#include "Game.h"
#include "Command.h"
#include "HexTile.hpp"
#include "GridFxController.h"

namespace hex
{
	struct CmdClearRingTile
	{
		static Command create(HexTile& pTile)
		{
			return Command{

				BlocksQ::Yes,
				Game::instance().fxController(),
				[action = CmdClearRingTile{ pTile }]
				(Command& pCmd)->void {
					action.run(pCmd);
				}
			};
		}

		HexTile& m_tile;
		void run(Command& pCmd) const;
	};


	void CmdClearRingTile::run(Command& pCmd) const
	{
		constexpr auto DT = 0.2f;		
		const auto scaleUp = cocos2d::ScaleTo::create(DT * 1.f/ 2.f, 1.15f);
		const auto scaleDown = cocos2d::ScaleTo::create(DT * 1.f/ 2.f, 0.01f);
		m_tile.getIdleFace().runAction(
			cocos2d::Sequence::create(
				scaleUp,
				cocos2d::CallFunc::create([&]()->void {
					pCmd.unblockQ();
				}),
				scaleDown,
				cocos2d::CallFunc::create([&]()->void {

					m_tile.getIdleFace().setScale(1.f);
					m_tile.assignColor(ColorId::None);
					m_tile.setState(TileState::None);
					
					pCmd.ends();
				}), nullptr
			)
		);
		m_tile.setState(TileState::Idle);
		m_tile.getLink().setVisible(true);
	}
}