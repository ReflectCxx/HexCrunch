
#include "Slider.h"
#include "HexGrid.h"
#include "HexTile.hpp"
#include "GridManager.h"

USING_NS_CC;

namespace hex
{
	GridManager::GridManager(HexGrid& pGrid)
		: m_grid(pGrid)
		, m_controller(pGrid)
	{ }


	void GridManager::swapSelection(const Slider& pSlider, const std::function<void()>& pOnEndCb) 
	{
		m_controller.swapTiles(pSlider.actor(), pSlider.follower(), pOnEndCb);
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
		const auto playerPos = m_grid.convertToWorldSpace(pSlider.actor().getPosition());
		const auto gridPosW = m_grid.convertToWorldSpace(Vec2::ZERO);
		const auto otherPosW = m_grid.convertToWorldSpace(pSlider.follower().getPosition());
		const auto d = (otherPosW - gridPosW);
		float theta = std::round(std::atan2(-d.y, d.x) * 180.0f / static_cast<float>(M_PI));
		if (theta <= 30.f || theta >= 150.f)
		{
			const auto theta = ((playerPos.x > gridPosW.x) ? 60.f : -60.f);
			m_controller.rotateGrid(theta);
		}
	}
}