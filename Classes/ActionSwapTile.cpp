
#include "HexTile.h"
#include "HexGrid.h"
#include "ActionSwapTile.h"


USING_NS_CC;

namespace {
	constexpr auto DT = hex::ANIM_SCALE * 0.25f;
}

namespace hex
{
	void ActionSwapTile::run(Command& pCmd) const
	{
		const auto& actorPos = m_tileA.getPosition();
		const auto& otherPos = m_tileB.getPosition();

		m_tileA.runAction(MoveTo::create(DT, otherPos));
		m_tileB.runAction(MoveTo::create(DT, actorPos));

		m_tileA.getForeground().setVisible(true);
		m_tileA.getClippedFace().setVisible(false);

		m_tileB.getForeground().setVisible(true);
		m_tileB.getClippedFace().setVisible(false);

		const auto runOn = [](HexTile& tile) {
			tile.runAction(
				Sequence::create(ScaleTo::create(DT / 2.f, 0.5),
					ScaleTo::create(DT / 2.f, 1.f),
					nullptr));
		};

		runOn(m_tileA);
		runOn(m_tileB);

		Game::instance().grid().scheduleOnce([=, &pCmd](float)
		{
			m_tileA.swapColors(m_tileB);
			m_tileA.setPosition(actorPos);
			m_tileB.setPosition(otherPos);

			m_tileA.getClippedFace().setVisible(true);
			m_tileA.getForeground().setVisible(false);

			m_tileB.getClippedFace().setVisible(true);
			m_tileB.getForeground().setVisible(false);
			pCmd.end();

		}, DT + 0.01f, "cb");
	}
}