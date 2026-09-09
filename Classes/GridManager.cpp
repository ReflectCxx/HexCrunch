
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


	void GridManager::swapSelection(Slider& pSlider, const std::function<void()>& pOnEndCb) 
	{
		m_controller.swapTiles(pSlider, [=, &pSlider]()->void
		{
			if (clearRingsMade()) 
			{
				pSlider.setActive(false);
				m_controller.pushCb([this, &pSlider]()->void
				{
					auto& ringsMat = Game::instance().grid().getHexagonRings();
					for (auto ri : m_ringsMadeIndices) {
						for (auto tile : ringsMat[ri]) {
							tile->setState(TileState::None);
						}
					}
					pSlider.setActive(true);
				});
			}
			else {
				pOnEndCb();
			}
		});
	}


	bool GridManager::clearRingsMade()
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

		m_ringsMadeIndices.clear();
		auto& ringsMat = Game::instance().grid().getHexagonRings();
		for (int i = 0; i < RING_COUNT; i++) {
			if (isMakingRing(ringsMat[i])) {
				m_ringsMadeIndices.push_back(i);
				m_controller.clearRingAtIndex(i);
			}
		}
		return !m_ringsMadeIndices.empty();
	}
}