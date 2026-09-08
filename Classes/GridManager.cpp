
#include "Slider.h"
#include "HexGrid.h"
#include "HexTile.hpp"
#include "Game.h"
#include "GridManager.h"

USING_NS_CC;

namespace hex
{
	void GridManager::swapSelection(const Slider& pSlider, const std::function<void()>& pOnEndCb) 
	{
		m_controller.swapTiles(pSlider, [=, &pSlider]() {
			if (checkIfRingMade()) {
				
			}
			else {
				pOnEndCb();
			}
		});
	}


	void GridManager::setRingTilesState(HexTile& ringTile, TileState state) const
	{
		auto nextTile = &ringTile;
		do {
			nextTile->setState(state);
			nextTile = static_cast<HexTile*>(nextTile->getNextRingTile());
		} while (nextTile != &ringTile);
	}


	void GridManager::correctOrientation(const Slider& pSlider)
	{
		auto& hexGrid = Game::instance().grid().getHexNode();
		const auto playerPos = hexGrid.convertToWorldSpace(pSlider.actor().getPosition());
		const auto gridPosW = hexGrid.convertToWorldSpace(Vec2::ZERO);
		const auto otherPosW = hexGrid.convertToWorldSpace(pSlider.follower().getPosition());
		const auto d = (otherPosW - gridPosW);
		float theta = std::round(std::atan2(-d.y, d.x) * 180.0f / static_cast<float>(M_PI));
		if (theta <= 30.f || theta >= 150.f)
		{
			const auto theta = ((playerPos.x > gridPosW.x) ? 60.f : -60.f);
			m_controller.rotateGrid(theta);
		}
	}


	bool GridManager::checkIfRingMade()
	{
		auto isMakingRing = [](const std::vector<HexTile*>& pRing)->bool 
		{
			const auto color = pRing[0]->getColorId();
			for (const auto tile : pRing) {
				if (tile->getColorId() != color) {
					return false;
				}
			}
			return true;
		};

		bool ringMade = false;
		auto& ringsMat = Game::instance().grid().getHexagonRings();
		for (int i = 0; i < RING_COUNT; i++) {
			if (isMakingRing(ringsMat[i])) {
				//remove ring.
				ringMade = true;
			}
		}
		return ringMade;
	}
}