
#include "HexGrid.h"
#include "HexTile.hpp"
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
		, m_currentSlideDir(Swipe::None)
		, m_lastSwappedInDir(Swipe::None)
		, m_lastSwappedIndices{ -1, -1 }
		, m_grid(pHexGrid)
		, m_actorTile(nullptr)
	{ }


	void TouchConsumer::init()
	{
		m_otherTileIndex = 1;
		m_actorTile = m_grid.getHexagonRings()[RING_COUNT - 2][0];
		updateOuterRingPathQ();
		highlightCurrentRing(Turn::On);
		m_grid.controller().correctOrientation(actorTile(), otherTile());
	}
}



namespace hex
{
	bool TouchConsumer::moveSliderTiles(const Swipe pDir)
	{
		switch (pDir){
		case Swipe::Up: return moveSelectionUp();
		case Swipe::Left: return moveSelectionLeft();
		case Swipe::Down: 	return moveSelectionDown();
		case Swipe::Right: return moveSelectionRight();
		default:return false;
		}
	}


	void TouchConsumer::onInputRecieved(const Swipe pDir, const bool pIsMockInput)
	{
		if (!m_grid.controller().isGridIdle()) {
			return;
		}
		if (pDir == Swipe::SingleTap) {
			trackTapToPredictNextSwap();
			swapSelection();
		}
		else
		{
			highlightBlockedTiles(Turn::Off);
			trackSwipeToPredictNextSwap(pDir);
			if constexpr (ENABLE_SAME_COLOR_SWAP) {
				moveSliderTiles(pDir);
			}
			else
			{
				auto prevoiusActor = m_actorTile;
				auto previousTileI = m_otherTileIndex;
				bool movedSuccessfully = false;

				do {
					movedSuccessfully = moveSliderTiles(pDir);
					if (actorTile().getColorId() == otherTile().getColorId()) {
						m_blockedTiles.insert(&actorTile());
						m_blockedTiles.insert(&otherTile());
					}
				} while (movedSuccessfully && actorTile().getColorId() == otherTile().getColorId());

				if (!movedSuccessfully) {
					undoSliderMove(prevoiusActor, previousTileI);
				}
				highlightBlockedTiles(Turn::On);
			}
			if (!pIsMockInput) {
				m_grid.controller().correctOrientation(actorTile(), otherTile());
			}
		}
	}
}



namespace hex
{
	void TouchConsumer::undoSliderMove(HexTile* pPreviousActor, const int pPrvOtherTileIndex)
	{
		highlightCurrentRing(Turn::Off);
		m_actorTile = pPreviousActor;
		m_otherTileIndex = pPrvOtherTileIndex;
		updateOuterRingPathQ();
		highlightCurrentRing(Turn::On);
	}


	void TouchConsumer::trackTapToPredictNextSwap()
	{
		if (m_lastSwappedInDir != Swipe::None && m_lastSwappedInDir == m_currentSlideDir) {
			m_sameDirSwapCount++;
		}
		else {
			m_sameDirSwapCount = 0;
			m_sameDirSlideCount = 0;
			m_lastSwappedInDir = Swipe::None;
		}
		m_lastSwappedInDir = m_currentSlideDir;
	}


	void TouchConsumer::trackSwipeToPredictNextSwap(const Swipe pDir)
	{
		if (pDir != m_currentSlideDir || pDir == Swipe::Up || pDir == Swipe::Down) {
			m_sameDirSlideCount = 0;
			m_lastSwappedInDir = Swipe::None;
		}
		else {
			if (m_sameDirSlideCount != m_sameDirSwapCount) {
				m_sameDirSwapCount = 0;
				m_lastSwappedInDir = Swipe::None;
			}
			m_sameDirSlideCount++;
		}
		m_currentSlideDir = pDir;
	}


	void TouchConsumer::highlightBlockedTiles(const Turn pFlag)
	{
		if (pFlag == Turn::Off) {
			for (auto t : m_blockedTiles) {
				if (t->getRingIndex() == actorTile().getRingIndex()) {
					t->setState(TileState::RingFace);
				}
				else {
					t->setState(TileState::Idle);
				}
			}
			m_blockedTiles.clear();
		}
		else {
			m_blockedTiles.erase(&actorTile());
			m_blockedTiles.erase(&otherTile());
			for (auto t : m_blockedTiles) {
				t->setState(TileState::Blocked);
			}
		}
	}
}



namespace hex
{
	void TouchConsumer::updateOuterRingPathQ()
	{
		const auto downTiles = actorTile().getOuterNeighbours();
		m_outerNeighbours.clear();
		for (auto t : downTiles) {
			m_outerNeighbours.push_back(static_cast<HexTile*>(t));
		}
		m_otherTileIndex = std::clamp(m_otherTileIndex, 0, (int)m_outerNeighbours.size() - 1);
	}


	void TouchConsumer::highlightCurrentRing(const Turn pFlag)
	{
		if (pFlag == Turn::On) {
			m_grid.controller().setRingTilesState(actorTile(), TileState::RingFace);
			actorTile().setState(TileState::Actor);
			otherTile().setState(TileState::Highlighted);
		}
		else {
			otherTile().setState(TileState::Idle);
			m_grid.controller().setRingTilesState(actorTile(), TileState::Idle);
		}
	}


	bool TouchConsumer::moveSelectionUp()
	{
		bool movedSuccessfully = false;
		highlightCurrentRing(Turn::Off);
		if (actorTile().getRingIndex() != 0)
		{
			moveActorUp();
			updateOuterRingPathQ();
			movedSuccessfully = true;
		}
		highlightCurrentRing(Turn::On);
		return movedSuccessfully;
	}


	bool TouchConsumer::moveSelectionDown()
	{
		bool movedSuccessfully = false;
		highlightCurrentRing(Turn::Off);
		if (actorTile().getRingIndex() < (RING_COUNT - 2))
		{
			moveActorDown();
			updateOuterRingPathQ();
			movedSuccessfully = true;
		}
		highlightCurrentRing(Turn::On);
		return movedSuccessfully;
	}



	void TouchConsumer::moveActorUp()
	{
		const auto upTiles = actorTile().getInnerNeighbours();
		if (upTiles.size() == 1) {
			m_actorTile = upTiles.back();
		}
		else {
			const auto gridPosW = m_grid.convertToWorldSpace(Vec2::ZERO);
			const auto tilePosW = m_grid.convertToWorldSpace(actorTile().getPosition());
			const auto i = (tilePosW.x > gridPosW.x ? 1 : 0);

			m_actorTile = upTiles[i];
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
				m_actorTile = downTiles[1];
			}
			else {
				const auto tilePosW = m_grid.convertToWorldSpace(otherTile().getPosition());
				const auto i = m_otherTileIndex + (tilePosW.x < gridPosW.x ? 1 : -1);
				m_actorTile = downTiles[std::clamp(i, 0, 2)];
			}
		}
		else {
			const auto tilePosW = m_grid.convertToWorldSpace(actorTile().getPosition());
			const auto i = (tilePosW.x > gridPosW.x ? 0 : 1);
			m_actorTile = downTiles[i];
		}
	}


	void TouchConsumer::swapSelection()
	{
		const auto actorTi = actorTile().getTileIndex();
		const auto otherTi = otherTile().getTileIndex();
		if (std::make_pair(actorTi, otherTi) == m_lastSwappedIndices ||
			std::make_pair(otherTi, actorTi) == m_lastSwappedIndices) {
			m_lastSwappedInDir = Swipe::None;
			m_sameDirSwapCount = 0;
		}
		m_lastSwappedIndices = { actorTi, otherTi };

		const auto onEndCb = [=]()-> bool
		{
			if (m_sameDirSwapCount > 1)
			{
				onInputRecieved(m_currentSlideDir, true);
				const auto ri = otherTile().getRingIndex();
				const auto ti = otherTile().getTileIndex();
				if (ti % (ri + 1) == 0) {
					m_sameDirSwapCount++;
					onInputRecieved(m_currentSlideDir, true);
				}
				m_grid.controller().correctOrientation(actorTile(), otherTile());
				return true;
			}
			return false;
		};
		m_grid.controller().swapSelection(actorTile(), otherTile(), onEndCb);
	}


	bool TouchConsumer::moveSelectionLeft()
	{
		if (m_otherTileIndex > 0) {
			otherTile().setState(TileState::Idle);
			m_otherTileIndex--;
			otherTile().setState(TileState::Highlighted);
		}
		else {
			actorTile().setState(TileState::RingFace);
			auto& outerTile = otherTile();
			m_outerNeighbours.pop_back();
			{
				const auto ri = actorTile().getRingIndex();
				const auto ti = actorTile().getTileIndex();
				if (ti % (ri + 1) == 0) {
					m_outerNeighbours.pop_back();
				}
			}
			m_actorTile = actorTile().getPrevoiusRingTile();
			actorTile().setState(TileState::Actor);
			const auto pt = outerTile.getPrevoiusRingTile();
			m_outerNeighbours.push_front(pt);
			{
				const auto ri = actorTile().getRingIndex();
				const auto ti = actorTile().getTileIndex();
				if (ti % (ri + 1) == 0) {
					auto pt = m_outerNeighbours.front()->getPrevoiusRingTile();
					m_outerNeighbours.push_front(pt);
				}
			}
			m_otherTileIndex = (m_outerNeighbours.size() - 1);
		}
		return true;
	}


	bool TouchConsumer::moveSelectionRight()
	{
		if (m_otherTileIndex < (m_outerNeighbours.size() - 1)) {
			otherTile().setState(TileState::Idle);
			m_otherTileIndex++;
			otherTile().setState(TileState::Highlighted);
		}
		else {
			actorTile().setState(TileState::RingFace);
			auto& outerTile = otherTile();
			m_outerNeighbours.pop_front();
			{
				const auto ri = actorTile().getRingIndex();
				const auto ti = actorTile().getTileIndex();
				if (ti % (ri + 1) == 0) {
					m_outerNeighbours.pop_front();
				}
			}
			m_actorTile = actorTile().getNextRingTile();
			actorTile().setState(TileState::Actor);
			const auto nt = outerTile.getNextRingTile();
			m_outerNeighbours.push_back(nt);
			{
				const auto ri = actorTile().getRingIndex();
				const auto ti = actorTile().getTileIndex();
				if (ti % (ri + 1) == 0) {
					auto nt = m_outerNeighbours.back()->getNextRingTile();
					m_outerNeighbours.push_back(nt);
				}
			}
			m_otherTileIndex = 0;
		}
		return true;
	}
}