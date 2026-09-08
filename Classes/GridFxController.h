#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include "Command.h"

namespace hex
{
	class HexTile;

	class GridFxController : public CommandController
	{
		void pushCb(const std::function<void()>& pCallBack);

	public:

		void update();
		void rotateGrid(const float);
		void swapTiles(HexTile&, HexTile&, const std::function<void()>& pOnEndCb);
	};
}