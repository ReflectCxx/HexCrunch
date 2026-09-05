#pragma once

#include "HexTile.hpp"

namespace hex
{
	class HexTState : public HexTile
	{
		bool initStateZero();

		bool turnOnIdle();
		bool turnOffIdle();

		bool turnOnActing();
		bool turnOffActing();

		bool turnOnClipped();
		bool turnOffClipped();

		bool turnOnBlocked();
		bool turnOffBlocked();

		bool turnOnHighlighted();
		bool turnOffHighlighted();

		virtual bool stateOnDeactivate() override;
		virtual bool stateOnActivate(TileState) override;

	public:

		HexTState(const ColorId pId, 
				  const int pRingIndex, 
				  const int pTileIndex) :HexTile(pId, pRingIndex, pTileIndex) { }
	};
}