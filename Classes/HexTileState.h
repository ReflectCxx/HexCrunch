#pragma once

namespace hex
{
	class HexTile;

	struct HexTileState
	{
		static bool initStateZero(HexTile&);

		static bool turnOnIdle(HexTile&);
		static bool turnOffIdle(HexTile&);
		
		static bool turnOnActing(HexTile&);
		static bool turnOffActing(HexTile&);
		
		static bool turnOnClipped(HexTile&);
		static bool turnOffClipped(HexTile&);
		
		static bool turnOnHighlighted(HexTile&);
		static bool turnOffHighlighted(HexTile&);
	};
}