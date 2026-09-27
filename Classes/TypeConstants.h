#pragma once

#include <map>
#include <array>
#include <vector>

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
		Stray,
		Actor,
		Follower,
		Blocked,
		RingFace,
	};
}


namespace hex
{
	constexpr auto HEX_6 = 6;

	using SectorColors = std::array<std::map<ColorId, int>, HEX_6>;

	class HexTile;
	using HexRingMatrix = std::vector<std::vector<HexTile*>>;
}