#pragma once

#include <functional>

#include "GridFxController.h"

namespace hex
{
	class Slider;
	class HexTile;
	class HexGrid;
	
	class GridManager
	{
		HexGrid& m_grid;
		GridFxController m_controller;

	public:

		GridManager(HexGrid& pGrid);
		
		void update();
		
		constexpr bool isGridIdle();
		constexpr CommandController& controller();

		void correctOrientation(const Slider&);
		void setRingTilesState(HexTile& pStartTile, TileState) const;

		void swapSelection(const Slider&, const std::function<void()>& pOnEndCb);
	};
}


namespace hex
{
	inline void GridManager::update() {
		m_controller.update();
	}

	constexpr bool GridManager::isGridIdle() {
		return (m_controller.getRunningCmd() == CmdKind::None);
	}

	constexpr CommandController& GridManager::controller() {
		return m_controller;
	}
}