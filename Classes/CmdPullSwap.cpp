
#include "CmdPullSwap.h"

USING_NS_CC;

namespace {
	constexpr auto DT = hex::ANIM_SCALE * 0.25f;
}

namespace hex
{
	void CmdPullSwap::run(Command& pCmd) const
	{
		const auto pos = m_tile.getPosition();
		const auto vis = m_tile.getLink().isVisible();
		m_tile.setPosition(m_pullTile->getPosition());
		m_tile.setState(TileState::Idle);
		m_tile.getLink().setVisible(vis);
		m_tile.runAction(Sequence::create(
			MoveTo::create(DT, pos),
			CallFunc::create([&]() {
				CCLOG("pulled tile: %d", m_tile.getTileIndex());
				pCmd.end();
			}), nullptr
		));
	}


	bool CmdPullSwap::init()
	{
		auto& neighbours = m_tile.getNeighbours();
		auto ts = std::vector<HexTile*>();
		for (auto t : neighbours) {
			if (t->getRingIndex() > m_tile.getRingIndex() &&
				t->getColorId() != ColorId::None) {
				ts.push_back(t);
			}
		}
		if (ts.empty()) {
			return false;
		}

		std::random_device rd;
		std::mt19937 rng(rd());
		std::shuffle(ts.begin(), ts.end(), rng);
		m_pullTile = ts.front();
		
		const auto color = m_pullTile->getColorId();
		m_pullTile->assignColor(ColorId::None);
		m_tile.assignColor(color);
		return true;
	}
}