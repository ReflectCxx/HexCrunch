
#include "Game.h"
#include "Slider.h"
#include "HexGrid.h"
#include "HexTAlgo.h"
#include "HexTile.hpp"
#include "GridManager.h"


USING_NS_CC;


namespace
{
	inline const std::string col_str(const hex::ColorId pColor)
	{
		switch (pColor) {
		case hex::ColorId::Red: return "RED";
		case hex::ColorId::Blue: return "BLUE";
		case hex::ColorId::Green: return "GREEN";
		case hex::ColorId::Yellow: return "YELLOW";
		case hex::ColorId::Purple: return "PURPLE";
		default: return "NONE";
		}
	}
}


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
		const auto cb = [=, &pSlider]()->void 
		{
			if (popRings(pSlider)) {
				pSlider.setActive(false);
			}
			else {
				Game::instance().gridSanityCheck();
				pOnEndCb();
			}
		};
		m_controller.pushSliderSwap(pSlider, cb);
	}


	void GridManager::correctOrientation(const Slider& pSlider, const std::function<void()>& pOnEndCb)
	{
		auto& hexGrid = Game::instance().grid().getHexNode();
		const auto playerPos = hexGrid.convertToWorldSpace(pSlider.actor().getPosition());
		const auto gridPosW = hexGrid.convertToWorldSpace(Vec2::ZERO);
		const auto otherPosW = hexGrid.convertToWorldSpace(pSlider.follower().getPosition());
		const auto d = (otherPosW - gridPosW);
		float theta = std::round(std::atan2(-d.y, d.x) * 180.0f / static_cast<float>(M_PI));

		if (theta <= 30.f || theta >= 150.f) {
			const auto theta = ((playerPos.x > gridPosW.x) ? 60.f : -60.f);
			m_controller.pushRotateGrid(theta, pOnEndCb);
		}
	}


	void GridManager::spawnTiles(Slider& pSlider)
	{
		Game::instance().gridSanityCheck();
		Game::instance().seedSpawningColors();

		m_controller.pushPauseFxQ();

		auto& hexRings = Game::instance().grid().getHexagonRings();
		for (int ri = 0; ri < RING_COUNT; ri++) {
			for (const auto t : hexRings[ri]) {
				if (t->getState() == TileState::Stray) {
					m_controller.pushSpawnTile(*t);
				}
			}
		}

		m_controller.pushCallback(
			[&]()->void {
				pSlider.setActive(true);
			}
		);
	}


	bool GridManager::popRings(Slider& pSlider)
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
		auto& hexRings = Game::instance().grid().getHexagonRings();

		for (int ri = 0; ri < RING_COUNT; ri++) {
			if (isMakingRing(hexRings[ri])) {
				m_ringIndex = ri;
				m_ringColor = hexRings[ri][0]->getColorId();
				m_controller.pushClearRing(ri);
				break;
			}
		}

		if (m_ringIndex == -1) {
			return false;
		}
		CCLOG("Ring made at index: { %lu }", m_ringIndex);

		m_controller.pushCallback(
			[&]()->void {
				doHexCrunch(pSlider);
			}
		);
		return true;
	}


	void GridManager::doHexCrunch(Slider& pSlider)
	{
		bool anyTileMoved = false;
		auto& hexRings = Game::instance().grid().getHexagonRings();
		for (int ri = m_ringIndex; ri < (RING_COUNT - 1); ri++)
		{
			std::vector<HexTile*> emptyTiles;
			for (auto tile : hexRings[ri]) {
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

		for (int ri = 0; ri < RING_COUNT; ri++) {
			for (const auto t : hexRings[ri]) {
				if (t->getColorId() == ColorId::None) {
					t->setState(TileState::Stray);
				}
			}
		}

		m_controller.pushCallback([&, anyTileMoved]()->void 
		{
			if (anyTileMoved) {
				doHexCrunch(pSlider);
			}
			else {
				assert(m_ringColor != ColorId::None);
				for (int ri = 0; ri < RING_COUNT; ri++) {
					for (const auto t : hexRings[ri]) {
						if (t->getColorId() == ColorId::None && t->getSpawnColor() == ColorId::None) {
							t->setSpawnColor(m_ringColor);
						}
					}
				}
				if (!popRings(pSlider)) {
					spawnTiles(pSlider);
				}
			}
		});
	}
}