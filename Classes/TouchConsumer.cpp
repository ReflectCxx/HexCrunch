
#include "HexGrid.h"
#include "HexTileUtils.h"
#include "TouchConsumer.h"

USING_NS_CC;

namespace
{
	constexpr auto ENABLE_SAME_COLOR_SWAP = false;
}

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
		, m_sameDirSwapCount(0)
		, m_sameDirSlideCount(0)
		, m_currentSlideDir(Swipe::kNone)
		, m_lastSwappedInDir(Swipe::kNone)
		, m_lastSwappedIndices{ -1, -1 }
		, m_grid(pHexGrid)
		, m_actorTile(nullptr)
	{ }


	void TouchConsumer::init()
	{
		m_otherTileIndex = 1;
		m_actorTile = m_grid.getHexagonRings()[RING_COUNT - 2][0];
		updateOuterRingPathQ();
		highlightCurrentRing(true);
		m_grid.controller().correctOrientation(actorTile(), otherTile());
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


	void TouchConsumer::trackTapToPredictNextSwap()
	{
		if (m_lastSwappedInDir != Swipe::kNone && m_lastSwappedInDir == m_currentSlideDir) {
			m_sameDirSwapCount++;
		}
		else {
			m_sameDirSwapCount = 0;
			m_sameDirSlideCount = 0;
			m_lastSwappedInDir = Swipe::kNone;
		}
		m_lastSwappedInDir = m_currentSlideDir;
	}


	void TouchConsumer::trackSwipeToPredictNextSwap(const Swipe pDir)
	{
		if (pDir != m_currentSlideDir || pDir == Swipe::kUp || pDir == Swipe::kDown) {
			m_sameDirSlideCount = 0;
			m_lastSwappedInDir = Swipe::kNone;
		}
		else {
			if (m_sameDirSlideCount != m_sameDirSwapCount) {
				m_sameDirSwapCount = 0;
				m_lastSwappedInDir = Swipe::kNone;
			}
			m_sameDirSlideCount++;
		}
		m_currentSlideDir = pDir;
	}


	void TouchConsumer::onInputRecieved(const Swipe pDir)
	{
		if (!m_grid.controller().isGridIdle()) {
			return;
		}
		if (pDir == Swipe::kSingleTap) {
			trackTapToPredictNextSwap();
			swapSelection();
		}
		else
		{
			trackSwipeToPredictNextSwap(pDir);
			if constexpr (ENABLE_SAME_COLOR_SWAP) {
				moveSliderTiles(pDir);
			}
			else {
				auto prevoiusActor = m_actorTile;
				auto previousTileI = m_otherTileIndex;
				bool movedSuccessfully = false;

				do {
					movedSuccessfully = moveSliderTiles(pDir);
				} while (movedSuccessfully &&
						 actorTile().getColorId() == otherTile().getColorId());

				if (!movedSuccessfully) {
					highlightCurrentRing(false);
					m_actorTile = prevoiusActor;
					m_otherTileIndex = previousTileI;
					updateOuterRingPathQ();
					highlightCurrentRing(true);
				}
			}
			m_grid.controller().correctOrientation(actorTile(), otherTile());
		}
	}
}


namespace hex
{
	void TouchConsumer::swapSelection()
	{
		const auto actorTi = actorTile().getTileIndex();
		const auto otherTi = otherTile().getTileIndex();
		if (std::make_pair(actorTi, otherTi) == m_lastSwappedIndices ||
			std::make_pair(otherTi, actorTi) == m_lastSwappedIndices) {
			m_lastSwappedInDir = Swipe::kNone;
			m_sameDirSwapCount = 0;
		}
		m_lastSwappedIndices = { actorTi, otherTi };

		const auto onEndCb = [=]()-> bool
		{
			if (m_sameDirSwapCount > 1) {
				onInputRecieved(m_currentSlideDir);
				m_grid.controller().correctOrientation(actorTile(), otherTile());
				return true;
			}
			CCLOG("Same dir swap count : %d", m_sameDirSwapCount);
			return false;
		};
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
			movedSuccessfully = true;
		}
		highlightCurrentRing(true);
		return movedSuccessfully;
	}


	bool TouchConsumer::moveSelectionDown()
	{
		bool movedSuccessfully = false;
		highlightCurrentRing(false);
		if (actorTile().getRingIndex() < (RING_COUNT - 2))
		{
			moveActorDown();
			updateOuterRingPathQ();
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