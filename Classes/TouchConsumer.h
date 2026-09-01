#pragma once

#include <deque>

namespace hex
{
	class HexGrid;
	class TouchConsumer
	{
		bool m_isLastSwipeUp;
		bool m_lastSwipeLeft;
		int m_otherTileIndex;
		Swipe m_currentSwipe;

		HexGrid& m_grid;

		HexTile* m_actorTile;
		std::deque<HexTile*> m_outerNeighbours;

		HexTile& actorTile();
		HexTile& otherTile();

		void swapSelectionAndMove();
		void updateOuterRingPathQ();
		void highlightCurrentRing(bool pStateOn);

		void moveActorUp();
		void moveActorDown();

		bool moveSelectionUp();
		bool moveSelectionLeft();
		bool moveSelectionDown();
		bool moveSelectionRight();

		bool moveSliderTiles(const Swipe pDir);

	public:

		TouchConsumer(HexGrid& pHexGrid);

		void init();
		void onInputRecieved(const Swipe pDir);
	};
}