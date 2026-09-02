
#include "HexGrid.h"
#include "HexTileUtils.h"
#include "TouchConsumer.h"

USING_NS_CC;

namespace hex
{
	inline HexTile& TouchConsumer::otherTile()
	{
		return (*m_outerNeighbours[m_otherTileIndex]);
	}

	inline HexTile& TouchConsumer::actorTile()
	{
		return *m_actorTile;
	}
}


namespace hex
{
	TouchConsumer::TouchConsumer(HexGrid& pHexGrid)
		: m_otherTileIndex(-1)
		, m_lastSwipeLeft(false)
		, m_isLastSwipeUp(true)
		, m_currentSwipe(Swipe::kNone)
		, m_grid(pHexGrid)
		, m_actorTile(nullptr)
	{ }


	void TouchConsumer::init()
	{
		m_otherTileIndex = 1;
		m_actorTile = m_grid.getHexagonRings()[RING_COUNT - 2][0];
		updateOuterRingPathQ();
		m_grid.controller().correctOrientation(actorTile(), otherTile());
		highlightCurrentRing(true);
	}
}



namespace hex
{
	bool TouchConsumer::moveSliderTiles(const Swipe pDir)
	{
		switch (pDir){
		case Swipe::kUp: return moveSelectionUp();
		case Swipe::kLeft: return moveSelectionLeft();
		case Swipe::kDown: 	return moveSelectionDown();
		case Swipe::kRight: return moveSelectionRight();
		default:return false;
		}
	}


	void TouchConsumer::onInputRecieved(const Swipe pDir)
	{
		if (!m_grid.controller().isGridIdle()) {
			return;
		}
		if (pDir == Swipe::kSingleTap) {
			swapSelectionAndMove();
			return;
		}

		m_currentSwipe = pDir;
		auto currentActor = m_actorTile;
		int curOtherTileIndex = m_otherTileIndex;
		bool movedSuccessfully = false;

		do {
			movedSuccessfully = moveSliderTiles(pDir);
			if (!movedSuccessfully && actorTile().getColorId() == otherTile().getColorId())
			{
				highlightCurrentRing(false);
				m_actorTile = currentActor;
				m_otherTileIndex = curOtherTileIndex;
				updateOuterRingPathQ();
				highlightCurrentRing(true);
			}
		} while (movedSuccessfully && actorTile().getColorId() == otherTile().getColorId());
		m_grid.controller().correctOrientation(actorTile(), otherTile());
	}
}


namespace hex
{
	void TouchConsumer::swapSelectionAndMove()
	{
		if (otherTile().getRingIndex() == RING_COUNT) {
			return;
		}
		const auto onEndCb = [this]() {};
		m_grid.controller().swapSelection(actorTile(), otherTile(), onEndCb);
	}


	void TouchConsumer::updateOuterRingPathQ()
	{
		const auto downTiles = actorTile().getOuterNeighbours();
		m_outerNeighbours.clear();
		for (auto t : downTiles) {
			m_outerNeighbours.push_back(static_cast<HexTile*>(t));
		}
		m_otherTileIndex = std::clamp(m_otherTileIndex, 0, (int)m_outerNeighbours.size() - 1);
	}


	void TouchConsumer::highlightCurrentRing(bool pTurnOn)
	{
		if (pTurnOn) {
			m_grid.controller().setRingTilesState(actorTile(), TileState::kClipped);
			actorTile().setState(TileState::kActing);
			otherTile().setState(TileState::kHighlighted);
			//if (otherTile().getRingIndex() == RING_COUNT) 
			//{
			//	auto& link = otherTile().getLink();
			//	if (m_currentSwipe == Swipe::kLeft) {
			//		const auto color = actorTile().getPrevoiusRingTile()->getColorId();
			//		HexTileUtils::drawLinkCapsule(, color, { LINK_WIDTH, LINK_HEIGHT });
			//	}
			//	else if (m_currentSwipe == Swipe::kRight) {
			//		const auto color = actorTile().getNextRingTile()->getColorId();
			//	}
			//}
		}
		else {
			otherTile().setState(TileState::kIdle);
			m_grid.controller().setRingTilesState(actorTile(), TileState::kIdle);
		}
	}


	bool TouchConsumer::moveSelectionUp()
	{
		bool movedSuccessfully = false;
		highlightCurrentRing(false);
		if (actorTile().getRingIndex() != 0)
		{
			moveActorUp();
			updateOuterRingPathQ();
			m_grid.controller().correctOrientation(actorTile(), otherTile());
			movedSuccessfully = true;
		}
		highlightCurrentRing(true);
		return movedSuccessfully;
	}


	bool TouchConsumer::moveSelectionDown()
	{
		bool movedSuccessfully = false;
		highlightCurrentRing(false);
		if (actorTile().getRingIndex() < (RING_COUNT - 1))
		{
			moveActorDown();
			updateOuterRingPathQ();
			m_grid.controller().correctOrientation(actorTile(), otherTile());
			movedSuccessfully = true;
		}
		highlightCurrentRing(true);
		return movedSuccessfully;
	}


	void TouchConsumer::moveActorUp()
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
	}


	void TouchConsumer::moveActorDown()
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
	}


	bool TouchConsumer::moveSelectionLeft()
	{
		if (m_otherTileIndex > 0) {
			otherTile().setState(TileState::kIdle);
			m_otherTileIndex--;
			otherTile().setState(TileState::kHighlighted);
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
			actorTile().setState(TileState::kActing);
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
		return true;
	}


	bool TouchConsumer::moveSelectionRight()
	{
		if (m_otherTileIndex < (m_outerNeighbours.size() - 1)) {
			otherTile().setState(TileState::kIdle);
			m_otherTileIndex++;
			otherTile().setState(TileState::kHighlighted);
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
			actorTile().setState(TileState::kActing);
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
		return true;
	}
}