#pragma once

namespace hex
{
	class HexTile;
	class HexGrid;

	class HexGridController
	{
		bool m_isGridIdle;
		HexGrid& m_grid;

	public:

		HexGridController(HexGrid& pGrid);
		
		GETB(GridIdle, m_isGridIdle)

		void swapSelection(HexTile& pActor, HexTile& pOther);

		void correctOrientation(HexTile& pActorT, HexTile& pOtherT);

		void setRingTilesState(HexTile& pStartTile, TileState pState);
	};
}