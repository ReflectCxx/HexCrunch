#pragma once

#include <set>
#include <deque>

#include "Constants.h"
#include "HexGrid.h"

namespace hex
{
	class HexTile;
	class TouchConsumer;
	class CommandController;

	class Slider
	{
	public:

		std::size_t m_followerIndex;

		int m_sameDirSwapCount;
		int m_sameDirSlideCount;

		Swipe m_currentSwipeDir;
		Swipe m_lastSwappedInDir;
		std::pair<int, int> m_lastSwappedIndices;

		HexTile* m_actorTile;
		std::set<HexTile*> m_blockedTiles;
		std::deque<HexTile*> m_outerNeighbours;

		HexGrid& m_grid;
		
		void trackTap();
		void updateOuterRingPathQ();
		void undoSliderMove(HexTile*, const int);
		
		void highlightCurrentRing(const Turn);
		void highlightBlockedTiles(const Turn = Turn::On);

	public:

		Slider(HexGrid&);

		HexTile& follower() const;
		constexpr HexTile& actor() const;
		constexpr CommandController& fxController();
		constexpr bool isReady() const;

		void alignWithGrid();
		void trackSwipe(const Swipe);
		void swapSelection(TouchConsumer&);

		void moveActorUp();
		void moveActorDown();
		bool moveActorLeft();
		bool moveActorRight();

		void init(HexTile*);
	};
}



namespace hex
{
	constexpr CommandController& Slider::fxController() {
		return m_grid.manager().controller();
	}

	constexpr bool Slider::isReady() const {
		return m_grid.manager().isGridIdle();
	}

	constexpr HexTile& Slider::actor() const {
		return *m_actorTile;
	}

	inline HexTile& Slider::follower() const {
		return (*m_outerNeighbours[m_followerIndex]);
	}

	inline void Slider::alignWithGrid() {
		m_grid.manager().correctOrientation(*this);
	}
}