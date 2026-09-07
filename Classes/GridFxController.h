#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include "Command.h"

namespace hex
{
	class HexTile;
	class HexGrid;

	class GridFxController : public CommandController
	{
		HexGrid& m_grid;

		void pushCb(const std::function<void()>& pCallBack);

	public:

		GridFxController(HexGrid&);

		void update();

		void rotateGrid(const float);

		void swapTiles(HexTile&, HexTile&, const std::function<void()>& pOnEndCb);
	};
}