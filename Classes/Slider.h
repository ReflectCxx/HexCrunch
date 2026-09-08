#pragma once

#include <set>
#include <deque>

#include "Constants.h"

namespace hex
{
	class HexTile;
	class TouchConsumer;

	class Slider
	{
	public:

		std::size_t m_followerIndex = - 1;

		int m_sameDirSwapCount = 0;
		int m_sameDirSlideCount = 0;

		Swipe m_currentSwipeDir = Swipe::None;
		Swipe m_lastSwappedInDir = Swipe::None;
		std::pair<int, int> m_lastSwappedIndices = { -1, -1 };

		HexTile* m_actorTile = nullptr;
		std::set<HexTile*> m_blockedTiles;
		std::deque<HexTile*> m_outerNeighbours;
		
		void trackTap();
		void updateOuterRingPathQ();
		void undoSliderMove(HexTile*, const int);
		
		void highlightCurrentRing(const Turn);
		void highlightBlockedTiles(const Turn = Turn::On);

	public:

		HexTile& follower() const;
		constexpr HexTile& actor() const;

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
	constexpr HexTile& Slider::actor() const {
		return *m_actorTile;
	}

	inline HexTile& Slider::follower() const {
		return (*m_outerNeighbours[m_followerIndex]);
	}
}