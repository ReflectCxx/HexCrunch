#pragma once

namespace hex
{
	class HexTile;

	struct HexTileState
	{
		static void onIdle(HexTile&);
		static void onClipped(HexTile&);
		static void onSelected(HexTile&);
		static void onHighlighted(HexTile&);
	};
}