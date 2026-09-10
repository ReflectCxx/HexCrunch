
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


	void GridManager::pullOuterRingTiles(Slider& pSlider)
	{
		auto& ringsMat = Game::instance().grid().getHexagonRings();

		bool success = false;
		for (int i = 0; i < RING_COUNT; i++) {
			for (auto tile : ringsMat[i]) {
				if (tile->getColorId() == ColorId::None) {
					if (i == RING_COUNT - 1) {
						tile->setState(TileState::None);
					}
					else if (m_controller.pushAcquireNeighbour(*tile)) {
						success = true;
					}
				}
			}
		}

		m_controller.pushCallback(
			[&, success]()->void {
				if (success) {
					for (const auto t : ringsMat[RING_COUNT - 1]) {
						if (t->getColorId() == ColorId::None) {
							t->assignColor(ColorId::Red);
							t->setState(TileState::Idle);
						}
					}
					pullOuterRingTiles(pSlider);
				}
				else if (!clearRingsMade(pSlider)) {
					pSlider.setActive(true);
				}
			}
		);
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

		auto count = 0;
		auto& ringsMat = Game::instance().grid().getHexagonRings();
		for (int i = 0; i < RING_COUNT; i++) {
			if (isMakingRing(ringsMat[i])) {
				count++;
				m_controller.pushClearRing(i);
			}
		}

		if (count == 0) {
			CCLOG("No rings to clear.");
			return false;
		}

		CCLOG("Rings made : { %s }", std::to_string(count).c_str());
		m_controller.pushCallback(
			[&]()->void {
				pullOuterRingTiles(pSlider);
			}
		);
		return true;
	}
}