
#include <algorithm>

#include "Game.h"
#include "Slider.h"
#include "HexGrid.h"
#include "HexTile.hpp"
#include "TouchConsumer.h"

USING_NS_CC;

namespace hex
{
	void Slider::init(HexTile* pActorTile)
	{
		m_followerIndex = 1;
		m_actorTile = pActorTile;
		updateOuterRingPathQ();
		setActive(true);
		alignWithGrid();
	}

	void Slider::setActive(bool pActive) {
		m_isActive = pActive;
		highlightSelection(pActive ? Turn::On : Turn::Off);
	}
}



namespace hex
{
	void Slider::alignWithGrid() const {
		Game::instance().gridManager().correctOrientation(*this);
	}

	void Slider::resetToPosition(HexTile* pPreviousActor, const int pPrevFollowerI)
	{
		highlightSelection(Turn::Off);
		m_actorTile = pPreviousActor;
		m_followerIndex = pPrevFollowerI;
		updateOuterRingPathQ();
		highlightSelection(Turn::On);
	}


	void Slider::updateOuterRingPathQ()
	{
		const auto outerTiles = actor().getOuterNeighbours();
		m_outerNeighbours.clear();
		for (auto tile : outerTiles) {
			m_outerNeighbours.push_back(tile);
		}
		m_followerIndex = std::clamp(m_followerIndex, std::size_t{ 0 }, m_outerNeighbours.size() - 1);
	}


	void Slider::highlightSelection(const Turn pFlag)
	{
		auto& grid = Game::instance().grid();
		if (pFlag == Turn::On) {
			grid.manager().setRingTilesState(actor(), TileState::RingFace);
			if (actor().getColorId() != ColorId::None) {
				actor().setState(TileState::Actor);
				follower().setState(TileState::Follower);
			}
		}
		else {
			follower().setState(TileState::Idle);
			grid.manager().setRingTilesState(actor(), TileState::Idle);
		}
	}


	void Slider::trackTap()
	{
		if (m_lastSwappedInDir != Swipe::None && m_lastSwappedInDir == m_currentSwipeDir) {
			m_sameDirSwapCount++;
		}
		else {
			m_sameDirSwapCount = 0;
			m_sameDirSlideCount = 0;
			m_lastSwappedInDir = Swipe::None;
		}
		m_lastSwappedInDir = m_currentSwipeDir;
	}


	void Slider::trackSwipe(const Swipe pDir)
	{
		if (pDir != m_currentSwipeDir || pDir == Swipe::Up || pDir == Swipe::Down) {
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
		m_currentSwipeDir = pDir;
	}


	void Slider::highlightBlockedTiles(const Turn pFlag)
	{
		if (pFlag == Turn::Off) {
			for (auto t : m_blockedTiles) {
				if (t->getRingIndex() == actor().getRingIndex()) {
					t->setState(TileState::RingFace);
				}
				else {
					t->setState(TileState::Idle);
				}
			}
			m_blockedTiles.clear();
		}
		else {
			m_blockedTiles.erase(&actor());
			m_blockedTiles.erase(&follower());
			for (auto t : m_blockedTiles) {
				t->setState(TileState::Blocked);
			}
		}
	}


	void Slider::swapSelection(TouchConsumer& pTouch)
	{
		const auto actorTi = actor().getTileIndex();
		const auto otherTi = follower().getTileIndex();
		trackTap();

		if (std::make_pair(actorTi, otherTi) == m_lastSwappedIndices ||
			std::make_pair(otherTi, actorTi) == m_lastSwappedIndices) {
			m_lastSwappedInDir = Swipe::None;
			m_sameDirSwapCount = 0;
		}
		m_lastSwappedIndices = { actorTi, otherTi };

		const auto onEndCb = [&pTouch, this]()-> bool
		{
			if (m_sameDirSwapCount > 1)
			{
				pTouch.onInputRecieved(m_currentSwipeDir, true);
				const auto ri = follower().getRingIndex();
				const auto ti = follower().getTileIndex();
				if (ti % (ri + 1) == 0) {
					m_sameDirSwapCount++;
					pTouch.onInputRecieved(m_currentSwipeDir, true);
				}
				alignWithGrid();
				return true;
			}
			return false;
		};
		Game::instance().grid().manager().swapSelection(*this, onEndCb);
	}
}



namespace hex
{
	void Slider::moveActorUp()
	{
		const auto upTiles = actor().getInnerNeighbours();
		if (upTiles.size() == 1) {
			m_actorTile = upTiles.back();
		}
		else {
			auto& grid = Game::instance().grid();
			const auto gridPosW = grid.getHexNode().convertToWorldSpace(Vec2::ZERO);
			const auto tilePosW = grid.getHexNode().convertToWorldSpace(actor().getPosition());
			const auto i = (tilePosW.x > gridPosW.x ? 1 : 0);

			m_actorTile = upTiles[i];
			const auto ri = actor().getRingIndex();
			const auto ti = actor().getTileIndex();
			if ((ti % (ri + 1)) == 0 && tilePosW.x < gridPosW.x) {
				m_followerIndex++;
			}
		}
	}


	void Slider::moveActorDown()
	{
		auto& grid = Game::instance().grid();
		const auto downTiles = actor().getOuterNeighbours();
		const auto gridPosW = grid.getHexNode().convertToWorldSpace(Vec2::ZERO);
		if (downTiles.size() == 3) {
			if (m_followerIndex == 1) {
				m_actorTile = downTiles[1];
			}
			else {
				const auto tilePosW = grid.getHexNode().convertToWorldSpace(follower().getPosition());
				const int i = m_followerIndex + (tilePosW.x < gridPosW.x ? 1 : -1);
				m_actorTile = downTiles[std::clamp(i, 0, 2)];
			}
		}
		else {
			const auto tilePosW = grid.getHexNode().convertToWorldSpace(actor().getPosition());
			const auto i = (tilePosW.x > gridPosW.x ? 0 : 1);
			m_actorTile = downTiles[i];
		}
	}


	bool Slider::moveActorLeft()
	{
		if (m_followerIndex > 0) {
			follower().setState(TileState::Idle);
			m_followerIndex--;
			follower().setState(TileState::Follower);
		}
		else {
			actor().setState(TileState::RingFace);
			auto& outerTile = follower();
			m_outerNeighbours.pop_back();
			{
				const auto ri = actor().getRingIndex();
				const auto ti = actor().getTileIndex();
				if (ti % (ri + 1) == 0) {
					m_outerNeighbours.pop_back();
				}
			}
			m_actorTile = actor().getPrevoiusRingTile();
			actor().setState(TileState::Actor);
			const auto pt = outerTile.getPrevoiusRingTile();
			m_outerNeighbours.push_front(pt);
			{
				const auto ri = actor().getRingIndex();
				const auto ti = actor().getTileIndex();
				if (ti % (ri + 1) == 0) {
					auto pt = m_outerNeighbours.front()->getPrevoiusRingTile();
					m_outerNeighbours.push_front(pt);
				}
			}
			m_followerIndex = (m_outerNeighbours.size() - 1);
		}
		return true;
	}


	bool Slider::moveActorRight()
	{
		if (m_followerIndex < (m_outerNeighbours.size() - 1))
		{
			follower().setState(TileState::Idle);
			m_followerIndex++;
			follower().setState(TileState::Follower);
		}
		else {
			actor().setState(TileState::RingFace);
			auto& outerTile = follower();
			m_outerNeighbours.pop_front();
			{
				const auto ri = actor().getRingIndex();
				const auto ti = actor().getTileIndex();
				if (ti % (ri + 1) == 0) {
					m_outerNeighbours.pop_front();
				}
			}
			m_actorTile = actor().getNextRingTile();
			actor().setState(TileState::Actor);
			const auto nt = outerTile.getNextRingTile();
			m_outerNeighbours.push_back(nt);
			{
				const auto ri = actor().getRingIndex();
				const auto ti = actor().getTileIndex();
				if (ti % (ri + 1) == 0) {
					auto nt = m_outerNeighbours.back()->getNextRingTile();
					m_outerNeighbours.push_back(nt);
				}
			}
			m_followerIndex = 0;
		}
		return true;
	}
}