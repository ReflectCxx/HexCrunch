#pragma once

#include <deque>

#include "Constants.h"

namespace hex
{
	class Game;
	class HexSolutions
	{
		friend Game;

	public:

		void seedColors(std::deque<ColorId>&);
	};
}