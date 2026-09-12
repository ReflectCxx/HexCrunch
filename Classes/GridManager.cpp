
#include "Game.h"
#include "Slider.h"
#include "HexGrid.h"
#include "HexTAlgo.h"
#include "HexTile.hpp"
#include "GridManager.h"


USING_NS_CC;


namespace hex
{
	void GridManager::setRingTilesState(HexTile& ringTile, TileState state) const
	{
		auto nextTile = &ringTile;
		do {
			if (nextTile->getColorId() != ColorId::None) {
				nextTile->setState(state);
			}
			nextTile = static_cast<HexTile*>(nextTile->getNextRingTile());
		} while (nextTile != &ringTile);
	}


	void GridManager::swapSelection(Slider& pSlider, const std::function<void()>& pOnEndCb) 
	{
		const auto cb = [=, &pSlider]()->void {
			if (clearRingsMade(pSlider)) {
				pSlider.setActive(false);
			}
			else {
				pOnEndCb();
			}
		};
		m_controller.pushSliderSwap(pSlider, cb);
	}


	void GridManager::correctOrientation(const Slider& pSlider)
	{
		auto& hexGrid = Game::instance().grid().getHexNode();
		const auto playerPos = hexGrid.convertToWorldSpace(pSlider.actor().getPosition());
		const auto gridPosW = hexGrid.convertToWorldSpace(Vec2::ZERO);
		const auto otherPosW = hexGrid.convertToWorldSpace(pSlider.follower().getPosition());
		const auto d = (otherPosW - gridPosW);
		float theta = std::round(std::atan2(-d.y, d.x) * 180.0f / static_cast<float>(M_PI));

		if (theta <= 30.f || theta >= 150.f){
			const auto theta = ((playerPos.x > gridPosW.x) ? 60.f : -60.f);
			m_controller.pushRotateGrid(theta);
		}
	}
	

	void GridManager::donePullingTiles(Slider& pSlider)
	{
		const auto cb = [&]()->void {
			if (!clearRingsMade(pSlider))
			{
				auto& ringsMat = Game::instance().grid().getHexagonRings();
				for (int ri = 0; ri < RING_COUNT; ri++) {
					for (const auto t : ringsMat[ri]) {
						if (t->getColorId() == ColorId::None) {
							m_controller.pushSpawnTile(*t);
						}
					}
				}

				m_controller.pushCallback([&]() {
					pSlider.setActive(true);
				});
			}
		};
		m_controller.pushCallback(cb);
	}


	void GridManager::pullOuterRingTiles(Slider& pSlider)
	{
		bool anyTileMoved = false;
		auto& ringsMat = Game::instance().grid().getHexagonRings();

		for (int ri = m_ringIndex; ri < (RING_COUNT - 1); ri++)
		{
			std::vector<HexTile*> emptyTiles;
			for (auto tile : ringsMat[ri]) {
				if (tile->getColorId() == ColorId::None) {
					emptyTiles.push_back(tile);
				}
			}

			auto claimed = HexAlgo<HexTile>::claimNeighboursColor(emptyTiles);
			for (auto [emptyT, neighbourT] : claimed) {
				m_controller.pushAcquireNeighbour(*emptyT, *neighbourT);
				anyTileMoved = true;
			}
		}

		if (anyTileMoved) 
		{
			for (int ri = m_ringIndex; ri < RING_COUNT; ri++) {
				for (auto t : ringsMat[ri]) {
					if (t->getColorId() == ColorId::None) {
						t->setState(TileState::Stray);
					}
				}
			}
			m_controller.pushCallback([&]() {
				pullOuterRingTiles(pSlider);
			});
		}
		else {
			donePullingTiles(pSlider);
		}
	}


	bool GridManager::clearRingsMade(Slider& pSlider)
	{
		auto isMakingRing = [](const std::vector<HexTile*>& pRing)->bool {
			const auto color = pRing[0]->getColorId();
			if (color == ColorId::None) {
				return false;
			}
			for (const auto tile : pRing) {
				if (tile->getColorId() != color) {
					return false;
				}
			}
			return true;
		};

		m_ringIndex = -1;
		m_ringColor = ColorId::None;
		auto& ringsMat = Game::instance().grid().getHexagonRings();

		for (int ri = 0; ri < RING_COUNT; ri++) {
			if (isMakingRing(ringsMat[ri])) {
				m_ringIndex = ri;
				m_ringColor = ringsMat[ri][0]->getColorId();
				m_controller.pushClearRing(ri);
				break;
			}
		}

		if (m_ringColor == ColorId::None) {
			CCLOG("No rings to clear.");
			return false;
		}
		CCLOG("Rings made at index: %d", std::to_string(m_ringIndex).c_str());

		m_controller.pushCallback(
			[&]()->void {
				pullOuterRingTiles(pSlider);
			}
		);
		return true;
	}
}