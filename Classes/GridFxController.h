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
		void pushCallback(const CallBack& pCallBack);
		void pushRotateGrid(const float);
		void pushClearRing(const int pIndex);
		void pushSliderSwap(const Slider&, const CallBack& pOnEndCb);
		bool pushAcquireNeighbour(HexTile&);
	};
}