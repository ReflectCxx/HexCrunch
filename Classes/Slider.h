#pragma once

#include <set>
#include <deque>

#include "Constants.h"
#include "HexGridController.h"

namespace hex
{
	class HexTile;
	class HexGrid;
	class TouchConsumer;

	class Slider
	{
	public:

		int m_followerIndex;

		int m_sameDirSwapCount;
		int m_sameDirSlideCount;

		Swipe m_currentSwipeDir;
		Swipe m_lastSwappedInDir;
		std::pair<int, int> m_lastSwappedIndices;

		HexTile* m_actorTile;
		std::set<HexTile*> m_blockedTiles;
		std::deque<HexTile*> m_outerNeighbours;

		HexGrid& m_grid;
		HexGridController m_controller;
		
		void trackTap();
		void updateOuterRingPathQ();
		void undoSliderMove(HexTile*, const int);
		
		void highlightCurrentRing(const Turn);
		void highlightBlockedTiles(const Turn = Turn::On);

	public:

		Slider(HexGrid&);

		HexTile& follower() const;
		constexpr HexTile& actor() const;
		constexpr bool isReady() const;
		constexpr HexGridController& controller();

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
	inline constexpr HexTile& Slider::actor() const {
		return *m_actorTile;
	}

	inline constexpr bool Slider::isReady() const {
		return m_controller.isGridIdle();
	}

	inline constexpr HexGridController& Slider::controller() {
		return m_controller;
	}

	inline HexTile& Slider::follower() const {
		return (*m_outerNeighbours[m_followerIndex]);
	}
}