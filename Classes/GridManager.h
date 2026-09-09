#pragma once

#include <functional>

#include "GridFxController.h"

namespace hex
{
	class Slider;
	class HexTile;
	
	class GridManager
	{
		std::vector<int> m_ringsMadeIndices = { RING_COUNT, -1 };

		GridFxController m_controller;

		bool clearRingsMade();

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