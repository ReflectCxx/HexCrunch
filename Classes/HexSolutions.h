#pragma once

#include <map>
#include <deque>
#include <array>

#include "Constants.h"

namespace hex
{
	struct HexSolutions
	{
		const HexRingMatrix& m_hexRings;
		
		const SectorColors getSectorColors();
		
		const std::array<int, HEX_6> getSectorSpawnCount();

		const std::array<std::vector<ColorId>, HEX_6> seedSectorSpawns();

		static void seedColors(std::deque<ColorId>&);
	};
}