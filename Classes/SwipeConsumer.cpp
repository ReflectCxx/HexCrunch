
#include "HexGrid.h"
#include "HexTileUtils.h"
#include "SwipeConsumer.h"

USING_NS_CC;

namespace hex
{
	inline HexTile& SwipeConsumer::otherTile()
	{
		return (*m_outerNeighbours[m_otherTileIndex]);
	}

	inline HexTile& SwipeConsumer::actorTile()
	{
		return *m_actorTile;
	}


	void SwipeConsumer::onDoubleTap()
	{
		m_grid.controller().swapSelection(actorTile(), otherTile());
	}
}


namespace hex
{
	SwipeConsumer::SwipeConsumer(HexGrid& pHexGrid)
		: m_otherTileIndex(-1)
		, m_isGridRotating(false)
		, m_isTileMoving(false)
		, m_grid(pHexGrid)
		, m_actorTile(nullptr)
	{ }


	void SwipeConsumer::init()
	{
		m_otherTileIndex = 1;
		m_actorTile = m_grid.getHexagonRings()[RING_COUNT - 2][0];
		initOuterRingsTiles();

		m_grid.controller().setRingTilesState(actorTile(), TileState::kClipped);
		m_grid.controller().setRingTilesState(otherTile(), TileState::kClipped);
		m_grid.controller().correctOrientation(actorTile(), otherTile());

		actorTile().setState(TileState::kSelected);
		otherTile().setState(TileState::kSelected);
	}


	void SwipeConsumer::onInputRecieved(Slide pDir)
	{
		if (!m_grid.controller().isGridIdle()) {
			return;
		}
		switch (pDir)
		{
			case Slide::kUp: {
				moveSelectionUp();
				break;
			}
			case Slide::kLeft: {
				moveSelectionLeft();
				break;
			}
			case Slide::kDown: {
				moveSelectionDown();
				break;
			}
			case Slide::kRight: {
				moveSelectionRight();
				break;
			}
			case Slide::kSingleTap: {
				onDoubleTap();
				break;
			}
			default:break;
		}
		m_grid.controller().correctOrientation(actorTile(), otherTile());
	}
}


namespace hex
{
	void SwipeConsumer::initOuterRingsTiles()
	{
		if (m_otherTileIndex >= 0 && m_otherTileIndex < m_outerNeighbours.size()) {
			otherTile().setState(TileState::kIdle);
		}
		const auto downTiles = actorTile().getOuterNeighbours();
		m_outerNeighbours.clear();
		for (auto t : downTiles) {
			m_outerNeighbours.push_back(static_cast<HexTile*>(t));
		}
		m_otherTileIndex = std::clamp(m_otherTileIndex, 0, (int)m_outerNeighbours.size() - 1);
	}


	void SwipeConsumer::moveSelectionUp()
	{	
		m_grid.controller().setRingTilesState(actorTile(), TileState::kIdle);
		m_grid.controller().setRingTilesState(otherTile(), TileState::kIdle);

		if (actorTile().getRingIndex() != 0)
		{
			const auto upTiles = actorTile().getInnerNeighbours();
			if (upTiles.size() == 1) {
				m_actorTile = static_cast<HexTile*>(upTiles.back());
			}
			else {
				const auto gridPosW = m_grid.convertToWorldSpace(Vec2::ZERO);
				const auto tilePosW = m_grid.convertToWorldSpace(actorTile().getPosition());
				const auto i = (tilePosW.x > gridPosW.x ? 1 : 0);

				m_actorTile = static_cast<HexTile*>(upTiles[i]);
				const auto ri = actorTile().getRingIndex();
				const auto ti = actorTile().getTileIndex();
				if ((ti % (ri + 1)) == 0 && tilePosW.x < gridPosW.x) {
					m_otherTileIndex++;
				}
			}
			initOuterRingsTiles();
			m_grid.controller().correctOrientation(actorTile(), otherTile());
		}

		m_grid.controller().setRingTilesState(actorTile(), TileState::kClipped);
		m_grid.controller().setRingTilesState(otherTile(), TileState::kClipped);

		actorTile().setState(TileState::kSelected);
		otherTile().setState(TileState::kSelected);
	}


	void SwipeConsumer::moveSelectionDown()
	{
		m_grid.controller().setRingTilesState(actorTile(), TileState::kIdle);
		m_grid.controller().setRingTilesState(otherTile(), TileState::kIdle);

		if (actorTile().getRingIndex() < (RING_COUNT - 2)) 
		{
			const auto downTiles = actorTile().getOuterNeighbours();
			const auto gridPosW = m_grid.convertToWorldSpace(Vec2::ZERO);
			if (downTiles.size() == 3) {
				if (m_otherTileIndex == 1) {
					m_actorTile = static_cast<HexTile*>(downTiles[1]);
				}
				else {
					const auto tilePosW = m_grid.convertToWorldSpace(otherTile().getPosition());
					const auto i = m_otherTileIndex + (tilePosW.x < gridPosW.x ? 1 : -1);
					m_actorTile = static_cast<HexTile*>(downTiles[std::clamp(i, 0, 2)]);
				}
			}
			else {
				const auto tilePosW = m_grid.convertToWorldSpace(actorTile().getPosition());
				const auto i = (tilePosW.x > gridPosW.x ? 0 : 1);
				m_actorTile = static_cast<HexTile*>(downTiles[i]);
			}
			initOuterRingsTiles();
			m_grid.controller().correctOrientation(actorTile(), otherTile());
		}

		m_grid.controller().setRingTilesState(actorTile(), TileState::kClipped);
		m_grid.controller().setRingTilesState(otherTile(), TileState::kClipped);

		actorTile().setState(TileState::kSelected);
		otherTile().setState(TileState::kSelected);
	}


	void SwipeConsumer::moveSelectionLeft()
	{
		if (m_otherTileIndex > 0) {
			otherTile().setState(TileState::kClipped);
			m_otherTileIndex--;
			otherTile().setState(TileState::kSelected);
		}
		else {
			actorTile().setState(TileState::kClipped);
			auto& outerTile = otherTile();
			m_outerNeighbours.pop_back();
			{
				const auto ri = actorTile().getRingIndex();
				const auto ti = actorTile().getTileIndex();
				if (ti % (ri + 1) == 0) {
					m_outerNeighbours.pop_back();
				}
			}
			m_actorTile = static_cast<HexTile*>(actorTile().getPrevoiusRingTile());
			actorTile().setState(TileState::kSelected);
			const auto pt = outerTile.getPrevoiusRingTile();
			m_outerNeighbours.push_front(static_cast<HexTile*>(pt));
			{
				const auto ri = actorTile().getRingIndex();
				const auto ti = actorTile().getTileIndex();
				if (ti % (ri + 1) == 0) {
					auto pt = m_outerNeighbours.front()->getPrevoiusRingTile();
					m_outerNeighbours.push_front(static_cast<HexTile*>(pt));
				}
			}
			m_otherTileIndex = (m_outerNeighbours.size() - 1);
		}
	}


	void SwipeConsumer::moveSelectionRight()
	{
		if (m_otherTileIndex < (m_outerNeighbours.size() - 1)) {
			otherTile().setState(TileState::kClipped);
			m_otherTileIndex++;
			otherTile().setState(TileState::kSelected);
		}
		else {
			actorTile().setState(TileState::kClipped);
			auto& outerTile = otherTile();
			m_outerNeighbours.pop_front();
			{
				const auto ri = actorTile().getRingIndex();
				const auto ti = actorTile().getTileIndex();
				if (ti % (ri + 1) == 0) {
					m_outerNeighbours.pop_front();
				}
			}
			m_actorTile = static_cast<HexTile*>(actorTile().getNextRingTile());
			actorTile().setState(TileState::kSelected);
			const auto nt = outerTile.getNextRingTile();
			m_outerNeighbours.push_back(static_cast<HexTile*>(nt));
			{
				const auto ri = actorTile().getRingIndex();
				const auto ti = actorTile().getTileIndex();
				if (ti % (ri + 1) == 0) {
					auto nt = m_outerNeighbours.back()->getNextRingTile();
					m_outerNeighbours.push_back(static_cast<HexTile*>(nt));
				}
			}
			m_otherTileIndex = 0;
		}
	}
}