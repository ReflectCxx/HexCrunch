#pragma once

#include <functional>

#include "GridFxController.h"

namespace hex
{
	class Slider;
	class HexTile;
	
	class GridManager
	{
		std::size_t m_ringIndex = -1;
		ColorId m_ringColor = ColorId::None;

		GridFxController m_controller;

		bool clearRingsMade(Slider&);
		void spawnTiles(Slider&);
		void pullOuterRingTiles(Slider&);
		
	public:
		
		constexpr GridFxController& controller();

		void update();
		void setRingTilesState(HexTile& pStartTile, TileState) const;
		void swapSelection(Slider&, const std::function<void()>& pOnEndCb);
		void correctOrientation(const Slider&, const std::function<void()>& pOnEndCb);
	};
}


namespace hex
{
	inline void GridManager::update() {
		m_controller.update();
	}


	constexpr GridFxController& GridManager::controller() {
		return m_controller;
	}
}