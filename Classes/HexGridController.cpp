
#include "HexGrid.h"
#include "HexTile.h"
#include "HexTileUtils.h"
#include "HexGridController.h"

#include "CommandManager.h"

USING_NS_CC;

namespace hex
{
	HexGridController::HexGridController(HexGrid& pGrid)
		: m_isGridIdle(true)
		, m_grid(pGrid)
	{ }


	void HexGridController::setRingTilesState(HexTile& pStartTile, TileState pState)
	{
		auto nextTile = &pStartTile;
		do {
			nextTile->setState(pState);
			nextTile = static_cast<HexTile*>(nextTile->getNextRingTile());
		} while (nextTile != &pStartTile);
	}


	void HexGridController::correctOrientation(HexTile& pActorT, HexTile& pOtherT)
	{
		const auto playerPos = m_grid.convertToWorldSpace(pActorT.getPosition());
		const auto gridPosW = m_grid.convertToWorldSpace(Vec2::ZERO);
		const auto otherPosW = m_grid.convertToWorldSpace(pOtherT.getPosition());
		const auto d = (otherPosW - gridPosW);
		float theta = std::round(std::atan2(-d.y, d.x) * 180.0f / static_cast<float>(M_PI));
		if (theta <= 30.f || theta >= 150.f)
		{
			const auto theta = ((playerPos.x > gridPosW.x) ? 60.0 : -60.0);
			m_grid.runAction(Sequence::create(RotateBy::create(0.5f, theta),
											  CallFunc::create([this] { m_isGridIdle = true; }),
											  nullptr));
			m_isGridIdle = false;
		}
	}


	void HexGridController::swapSelection(HexTile& pActorT, HexTile& pOtherT, const std::function<void()>& pOnEndCb)
	{
		constexpr auto DT = 0.25f;
		const auto& actorPos = pActorT.getPosition();
		const auto& otherPos = pOtherT.getPosition();

		pActorT.runAction(MoveTo::create(DT, otherPos));
		pOtherT.runAction(MoveTo::create(DT, actorPos));

		const auto runScale = [](HexTile& tile) {
			tile.runAction(
				Sequence::create(
					ScaleTo::create(DT / 2.f, 0.5),
					ScaleTo::create(DT / 2.f, 1.f), nullptr));
		};

		runScale(pActorT);
		runScale(pOtherT);

		pActorT.getClipped().setVisible(false);
		pActorT.getForeground().setVisible(true);
		pOtherT.getClipped().setVisible(false);
		pOtherT.getForeground().setVisible(true);

		m_isGridIdle = false;
		m_grid.scheduleOnce([=, &pActorT, &pOtherT](float)
		{
			HexTileUtils::swapTileColor(&pActorT, &pOtherT);
			
			pActorT.setPosition(actorPos);
			pOtherT.setPosition(otherPos);

			pActorT.getClipped().setVisible(true);
			pActorT.getForeground().setVisible(false);
			pOtherT.getClipped().setVisible(true);
			pOtherT.getForeground().setVisible(false);

			pOnEndCb();
			m_isGridIdle = true;
		}, DT + 0.01, "cb");
	}
}