
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
		m_tile.setPosition(m_pullFromPos);

		auto resetV = m_tile.getLink().isVisible();
		if (m_tile.getState() == TileState::None) {
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

		const auto otherTile = ts.front();
		m_pullFromPos = otherTile->getPosition();
		m_tile.swapColor(*otherTile, false);		
		return true;
	}
}