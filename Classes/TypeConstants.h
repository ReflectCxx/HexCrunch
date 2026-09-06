#pragma once

#include <vector>

namespace hex
{
	class HexTile;
	using NeighboursMat = std::vector<std::vector<int>>;
	using HexRingMatrix = std::vector<std::vector<HexTile*>>;
}


namespace hex
{
	enum class Turn {
		On,
		Off
	};

	enum class CmdKind {
		None,
		SpawnTile,
		SwapTiles,
		PullDownTile
	};

	enum class ColorId {
		None,
		Red,
		Blue,
		Green,
		Purple,
		Yellow
	};

	enum class Swipe {
		None,
		Up,
		Left,
		Down,
		Right,
		SingleTap
	};

	enum class TileState {
		None,
		Idle,
		Actor,
		Blocked,
		RingFace,
		Highlighted
	};
}