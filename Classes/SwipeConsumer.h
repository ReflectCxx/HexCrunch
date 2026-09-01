#pragma once

#include <deque>

namespace hex
{
	class HexGrid;
	class SwipeConsumer
	{
		int m_otherTileIndex;
		bool m_isGridRotating;
		bool m_isTileMoving;

		HexGrid& m_grid;

		HexTile* m_actorTile;
		std::deque<HexTile*> m_outerNeighbours;

		HexTile& actorTile();
		HexTile& otherTile();
		
		void initOuterRingsTiles();		
		void onDoubleTap();
		void moveSelectionUp();
		void moveSelectionLeft();
		void moveSelectionDown();
		void moveSelectionRight();

	public:

		SwipeConsumer(HexGrid& pHexGrid);

		void init();
		void onInputRecieved(Slide pDir);
	};
}