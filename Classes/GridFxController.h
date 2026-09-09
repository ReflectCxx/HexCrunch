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

		void update();
		void pushCb(const std::function<void()>& pCallBack);

		void rotateGrid(const float);
		void clearRingAtIndex(const int);
		void swapTiles(HexTile&, HexTile&);
		void swapTiles(const Slider&, const std::function<void()>& pOnEndCb);
	};
}