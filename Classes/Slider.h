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
		bool m_isActive = true;
		int m_sameDirSwapCount = 0;
		int m_sameDirSlideCount = 0;
		std::size_t m_followerIndex = -1;

		Swipe m_currentSwipeDir = Swipe::None;
		Swipe m_lastSwappedInDir = Swipe::None;
		std::pair<int, int> m_lastSwappedIndices = { -1, -1 };

		HexTile* m_actorTile = nullptr;
		std::set<HexTile*> m_blockedTiles;
		std::deque<HexTile*> m_outerNeighbours;
		
		void trackTap();

	public:

		HexTile& follower() const;

		constexpr HexTile& actor() const;
		constexpr bool isActive() const;
		constexpr int followerIndex() const;
		constexpr std::set<HexTile*>& blockedTiles();

		void moveActorUp();
		void moveActorDown();
		bool moveActorLeft();
		bool moveActorRight();

		void init(HexTile*);
		void setActive(bool);
		void alignWithGrid() const;
		void updateOuterRingPathQ();

		void trackSwipe(const Swipe);
		void swapSelection(TouchConsumer&);
		void resetToPosition(HexTile*, const int);
		void highlightSelection(const Turn);
		void highlightBlockedTiles(const Turn = Turn::On);
	};
}



namespace hex
{
	constexpr HexTile& Slider::actor() const {
		return *m_actorTile;
	}

	constexpr bool Slider::isActive() const {
		return m_isActive;
	}

	constexpr int Slider::followerIndex() const {
		return m_followerIndex;
	}

	constexpr std::set<HexTile*>& Slider::blockedTiles() {
		return m_blockedTiles;
	}

	inline HexTile& Slider::follower() const {
		return (*m_outerNeighbours[m_followerIndex]);
	}
}