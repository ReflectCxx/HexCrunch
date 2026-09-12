
#include "CmdPullSwap.h"

USING_NS_CC;

namespace {
	constexpr auto DT = hex::ANIM_SCALE * 0.25f;
}

namespace hex
{
	CmdPullSwap::CmdPullSwap(HexTile& pTile, HexTile& pPullFrom)
		: m_tile(pTile)
		, m_pullFromPos(pPullFrom.getPosition())
	{
		m_tile.swapColor(pPullFrom, false);
		pPullFrom.setState(TileState::None);
	}


	void CmdPullSwap::run(Command& pCmd) const
	{
		const auto pos = m_tile.getPosition();
		m_tile.setPosition(m_pullFromPos);

		auto resetV = m_tile.getLink().isVisible();
		if (m_tile.getState() == TileState::None || m_tile.getState() == TileState::Stray) {
			m_tile.setState(TileState::Idle);
			m_tile.getLink().setVisible(true);
			resetV = false;
		}
		else {
			m_tile.refreshView();
		}
		
		m_tile.runAction(Sequence::create(
			MoveTo::create(DT, pos),
			CallFunc::create([&, resetV]() {
				m_tile.getLink().setVisible(resetV);
				pCmd.end();
			}), nullptr
		));

		m_tile.runAction(
			Sequence::create(
				ScaleTo::create(DT / 2.f, BOUNCE_SCALE),
				ScaleTo::create(DT / 2.f, 1.f),
				nullptr
			)
		);
	}
}