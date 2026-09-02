#pragma once

#include <deque>

namespace hex
{
	class HexGrid;
	class TouchConsumer
	{
		int m_otherTileIndex;

		int m_sameDirSwapCount;
		int m_sameDirSlideCount;

		Swipe m_currentSlideDir;
		Swipe m_lastSwappedInDir;
		std::pair<int, int> m_lastSwappedIndices;

		HexGrid& m_grid;

		HexTile* m_actorTile;
		std::deque<HexTile*> m_outerNeighbours;

		HexTile& actorTile();
		HexTile& otherTile();

		void moveActorUp();
		void moveActorDown();
		bool moveSelectionUp();
		bool moveSelectionLeft();
		bool moveSelectionDown();
		bool moveSelectionRight();

		void updateOuterRingPathQ();
		void highlightCurrentRing(bool pStateOn);

		void swapSelection();
		bool moveSliderTiles(const Swipe pDir);

		void trackTapToPredictNextSwap();
		void trackSwipeToPredictNextSwap(const Swipe pDir);
		
	public:

		TouchConsumer(HexGrid& pHexGrid);

		void init();
		void onInputRecieved(const Swipe pDir);
	};
}