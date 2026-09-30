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
		
		void balanceStrayColors() const;

		const SectorColors getSectorColors() const;
		
		const SectorColors getSectorStrayColors() const;

		void seedSpawnColors(const SectorColors& perSectorStrayColorN) const;

		void seedColors(const std::vector<int>& pSectorColorN) const;

		static void log(const SectorColors sector);

	};
}