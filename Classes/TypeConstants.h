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
		No,   // Doesn't block the queue.
		Yes,  // Blocks the queue until this command finishes.
		Join, // Waits for all running commands to finish before dispatch.
	};

	enum class CmdState {
		None,
		Ready,
		Queued,
		Running,
		Expired
	};

	//enum class CmdKind {
	//	None,
	//	CallBack,
	//	SpawnTile,
	//	ClearTile,
	//	PullSwap,
	//	SliderSwap,
	//	RotateGrid,
	//	PullDownTile
	//};

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
		Follower,
		Blocked,
		RingFace,
		Highlighted
	};
}