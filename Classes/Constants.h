#pragma once

#include "Defines.h"
#include "AssetStrings.h"
#include "TypeConstants.h"


namespace hex
{
	constexpr auto SCR_WIDTH = /*1080.f; //*/1800.f;
	constexpr auto SCR_HEIGHT = /*2400.f; //*/2880.f;

	constexpr auto HEX_6 = 6;
	constexpr auto RING_COUNT = 5;

	constexpr auto EDGE_GAP = 35.f;
	constexpr auto WIN_SCALE = 0.7f;
	
	constexpr auto SQRT_3 = 1.7320508075688772f;

	constexpr auto GRID_WIDTH = (SCR_WIDTH - 2.f * EDGE_GAP);
	constexpr auto GRID_HEIGHT = (GRID_WIDTH * 2.f) / SQRT_3;

	constexpr auto HEX_WIDTH = GRID_HEIGHT / (2.f * RING_COUNT + 1.f);
	constexpr auto HEX_RAD = HEX_WIDTH / SQRT_3;
	constexpr auto HEX_HEIGHT = 2.f * HEX_RAD;
	constexpr auto HEX_BORDER = 6.0;
	constexpr auto BOUNCE_SCALE = 0.93f;
	constexpr auto CORNER_RAD = 15.f;
	constexpr auto TILE_HIGHLIGHT_ALPHA = 0.90;

	//Not scalable across resolutions.
	constexpr auto LINK_WIDTH = 100.f;
	constexpr auto LINK_HEIGHT = 20.f;
	constexpr auto CLIP_WIDTH = LINK_WIDTH * 1.1f;
	constexpr auto CLIP_HEIGHT = LINK_HEIGHT * 1.5;
}