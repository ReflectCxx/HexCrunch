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

	enum class BlocksQ {
		No,
		Yes
	};

	enum class CmdState {
		None,
		Ready,
		Queued,
		Running,
		Expired
	};

	enum class CmdKind {
		None,
		CallBack,
		SpawnTile,
		SwapTiles,
		RotateGrid,
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