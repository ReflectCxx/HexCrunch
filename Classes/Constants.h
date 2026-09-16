#pragma once

#include "Defines.h"
#include "TypeConstants.h"

namespace hex
{
	constexpr auto SCALE = 1.f;
	constexpr auto ANIM_SCALE = 1.f;
	constexpr auto WIN_SCALE = 0.7f;

	constexpr auto EDGE_GAP = SCALE * 35.f;
	constexpr auto SCR_WIDTH = SCALE * 1800.f; //*/1080.f;
	constexpr auto SCR_HEIGHT = SCALE * 2880.f; //*/2400.f;

	constexpr auto HEX_6 = 6;
	constexpr auto RING_COUNT = 5;
	constexpr auto SQRT_3 = 1.7320508f;

	constexpr auto GRID_WIDTH = (SCR_WIDTH - 2.f * EDGE_GAP);
	constexpr auto GRID_HEIGHT = (GRID_WIDTH * 2.f) / SQRT_3;

	constexpr auto HEX_WIDTH = GRID_HEIGHT / (2.f * RING_COUNT + 1.f);
	constexpr auto HEX_RAD = HEX_WIDTH / SQRT_3;
	constexpr auto HEX_HEIGHT = 2.f * HEX_RAD;

	constexpr auto CORNER_RAD = 15.f;
	constexpr auto HEX_BORDER = SCALE * 6.0;
	constexpr auto BOUNCE_SCALE = 0.93f;
	constexpr auto TILE_HIGHLIGHT_ALPHA = 0.90;

	constexpr auto LINK_WIDTH = SCALE * 100.f;
	constexpr auto LINK_HEIGHT = SCALE * 20.f;
	constexpr auto LINK_CLIP_W = LINK_WIDTH * 1.1f;
	constexpr auto LINK_CLIP_H = LINK_HEIGHT * 1.5;
}