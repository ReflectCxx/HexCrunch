#pragma once

#include <functional>

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
		
		constexpr bool isGridIdle() const;

		void correctOrientation(HexTile& pActorT, HexTile& pOtherT);
		void setRingTilesState(HexTile& pStartTile, TileState pState) const;
		void swapSelection(HexTile& pActor, HexTile& pOther, const std::function<void()>& pOnEndCb);
	};
}


namespace hex
{
	inline constexpr bool HexGridController::isGridIdle() const {
		return m_isGridIdle;
	}
}