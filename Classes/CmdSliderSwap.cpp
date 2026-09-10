
#include "HexGrid.h"
#include "HexTile.hpp"
#include "CmdSliderSwap.h"


USING_NS_CC;

namespace {
	constexpr auto DT = hex::ANIM_SCALE * 0.25f;
}

namespace hex
{
	void CmdSliderSwap::setZOrder(int pZOdr)
	{
		auto& grid = Game::instance().grid();

		m_actor.retain();
		if (pZOdr == -1) {
			const auto curZ = m_actor.getLocalZOrder();
			m_actor.removeFromParentAndCleanup(false);
			m_actor.getPrevoiusRingTile()->getLink().setVisible(false);
			grid.getHexBGNode().addChild(&m_actor, curZ - 1);
		}
		else {
			m_actor.removeFromParentAndCleanup(false);
			m_actor.getPrevoiusRingTile()->getLink().setVisible(true);
			grid.getHexNode().addChild(&m_actor, pZOdr);
		}
		m_actor.release();
	}


	void CmdSliderSwap::onEnd(Command& pCmd, int pZOdr, 
							const Vec2& pActorPos, const Vec2& pOtherPos)
	{
		setZOrder(pZOdr);

		m_actor.swapColors(m_follower);
		m_actor.setPosition(pActorPos);
		m_follower.setPosition(pOtherPos);

		m_actor.getClippedFace().setVisible(true);
		m_actor.getIdleFace().setVisible(false);

		m_follower.getClippedFace().setVisible(true);
		m_follower.getIdleFace().setVisible(false);
		pCmd.end();
	}


	void CmdSliderSwap::run(Command& pCmd)
	{
		const auto& actorPos = m_actor.getPosition();
		const auto& otherPos = m_follower.getPosition();
		const auto zOrder = m_actor.getLocalZOrder();

		setZOrder(-1);

		m_actor.runAction(MoveTo::create(DT, otherPos));
		m_follower.runAction(MoveTo::create(DT, actorPos));

		m_actor.getIdleFace().setVisible(true);
		m_actor.getClippedFace().setVisible(false);

		m_follower.getIdleFace().setVisible(true);
		m_follower.getClippedFace().setVisible(false);

		const auto runOn = [](HexTile& tile) {
			tile.runAction(
				Sequence::create(
					ScaleTo::create(DT / 2.f, 0.5),
					ScaleTo::create(DT / 2.f, 1.f),
					nullptr
				)
			);
		};

		runOn(m_actor);
		runOn(m_follower);

		Game::instance().grid().scheduleOnce(
			[=, &pCmd](float) { 
				onEnd(pCmd, zOrder, actorPos, otherPos);
			}, DT + 0.01f, "cb"
		);
	}
}