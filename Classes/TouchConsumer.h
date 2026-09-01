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
		
		void moveSelectionUp();
		void moveSelectionLeft();
		void moveSelectionDown();
		void moveSelectionRight();
		void swapSelectionAndMove();
		void initOuterRingsTiles();

	public:

		TouchConsumer(HexGrid& pHexGrid);

		void init();
		void onInputRecieved(Swipe pDir);
	};
}