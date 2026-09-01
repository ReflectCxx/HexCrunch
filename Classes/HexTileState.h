#pragma once

namespace hex
{
	class HexTile;

	struct HexTileState
	{
		static void setToIdle(HexTile&);
		static void setToActing(HexTile&);
		static void setToClipped(HexTile&);
		static void setToHighlighted(HexTile&);
	};
}