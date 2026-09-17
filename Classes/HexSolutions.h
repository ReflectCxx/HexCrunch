#pragma once

#include <deque>

#include "Constants.h"

namespace hex
{
	struct HexSolutions
	{
		const HexRingMatrix& m_hexRings;

		const std::string scanSectors();
		static void seedColors(std::deque<ColorId>&);
	};
}