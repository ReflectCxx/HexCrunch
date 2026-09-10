#pragma once

#include <functional>

#include "GridFxController.h"

namespace hex
{
	class Slider;
	class HexTile;
	
	class GridManager
	{
		GridFxController m_controller;
		std::vector<std::pair<std::size_t, ColorId>> m_ringsMade;

		bool clearRingsMade(Slider&);
		void pullOuterRingTiles(Slider&);

	public:
		
		constexpr GridFxController& controller();

		void update();
		void correctOrientation(const Slider&);
		void setRingTilesState(HexTile& pStartTile, TileState) const;
		void swapSelection(Slider&, const std::function<void()>& pOnEndCb);
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