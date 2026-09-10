
#include "Slider.h"
#include "HexGrid.h"
#include "HexTile.hpp"
#include "Game.h"
#include "GridManager.h"

USING_NS_CC;

namespace hex
{
	void GridManager::setRingTilesState(HexTile& ringTile, TileState state) const
	{
		auto nextTile = &ringTile;
		do {
			nextTile->setState(state);
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


	bool GridManager::clearRingsMade(Slider& pSlider)
	{
		auto isMakingRing = [](const std::vector<HexTile*>& pRing)->bool {
			const auto color = pRing[0]->getColorId();
			for (const auto tile : pRing) {
				if (tile->getColorId() != color) {
					return false;
				}
			}
			return true;
		};

		auto& ringsMat = Game::instance().grid().getHexagonRings();
		for (int ri = 0; ri < RING_COUNT; ri++) {
			if (isMakingRing(ringsMat[ri])) {
				m_ringsMade.push_back({ ri, ringsMat[ri][0]->getColorId() });
				m_controller.pushClearRing(ri);
			}
		}

		if (m_ringsMade.empty()) {
			CCLOG("No rings to clear.");
			return false;
		}

		CCLOG("Rings made : { %s }", std::to_string(m_ringsMade.size()).c_str());
		m_controller.pushCallback(
			[&]()->void {
				pullOuterRingTiles(pSlider);
			}
		);
		return true;
	}


	void GridManager::pullOuterRingTiles(Slider& pSlider)
	{
		auto& ringsMat = Game::instance().grid().getHexagonRings();
		
		bool anyTileMoved = false;
		const auto ringIndex = m_ringsMade.back().first;
		const auto ringColor = m_ringsMade.back().second;

		for (int ri = ringIndex; ri < RING_COUNT; ri++) 
		{
			for (auto tile : ringsMat[ri]) {
				if (tile->getColorId() == ColorId::None)
				{
					if (ri == RING_COUNT - 1) {
						tile->assignColor(ringColor);
						tile->setState(TileState::None);
					}
					else if (m_controller.pushAcquireNeighbour(*tile)) {
						anyTileMoved = true;
					}
				}
			}
		}

		if (!anyTileMoved) {
			m_ringsMade.pop_back();
		}
		bool continuePull = !m_ringsMade.empty();

		m_controller.pushCallback(
			[&, continuePull]()->void {
				if (continuePull) {
					pullOuterRingTiles(pSlider);
				}
				else if (!clearRingsMade(pSlider)) {
					for (auto tile : ringsMat[RING_COUNT - 1]) {
						if (tile->getState() == TileState::None) {
							m_controller.pushSpawnTile(*tile);
						}
					}

					m_controller.pushCallback([&]() { 
						pSlider.setActive(true); 
					});
				}
			}
		);
	}
}