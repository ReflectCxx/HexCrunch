#pragma once

#include <vector>

namespace hex
{
	class HexTile;
	using NeighboursMat = std::vector<std::vector<int>>;
	using HexgonRingMatrix = std::vector<std::vector<HexTile*>>;
}


namespace hex
{
	enum class ColorId
	{
		kNone,
		kRed,
		kBlue,
		kGreen,
		kPurple,
		kYellow
	};


	enum class Swipe {
		kNone,
		kUp,
		kLeft,
		kDown,
		kRight,
		kSingleTap
	};

	enum class TileState
	{
		kNone,
		kIdle,
		kClipped,
		kSelected,
		kHighlighted
	};


	enum class ExecutionKind
	{
		kNone,
		kSynchronous,
		kASynchronous
	};


	enum class CmdKind
	{
		kNone = -1,
		kSpawnTile,
		kSwapTiles,
		kPullDownTile,
		kCount
	};
}