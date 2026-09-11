#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include "Command.h"

namespace hex
{
	class Slider;
	class HexTile;
	class GridManager;

	class GridFxController : public CommandController
	{
		friend GridManager;

		using CallBack = std::function<void()>;

		void update();
		void pushSpawnTile(HexTile&);
		void pushCallback(const CallBack&);
		void pushSliderSwap(const Slider&, const CallBack&);

		void pushClearRing(const int pIndex);
		void pushRotateGrid(const float);
		void pushAcquireNeighbour(HexTile&, HexTile& pPullFrom);
	};
}