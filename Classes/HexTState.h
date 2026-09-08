#pragma once

#include "HexTile.hpp"
#include "StateMachine.h"

namespace hex
{
	class HexTState : public HexTile, public StateMachine<TileState, HexTState>
	{
		bool initStateZero();

		bool turnOnIdle();
		bool turnOffIdle();

		bool turnOnActing();
		bool turnOffActing();

		bool turnOnFollower();
		bool turnOffFollower();

		bool turnOnRingFace();
		bool turnOffRingFace();

		bool turnOnBlocked();
		bool turnOffBlocked();

		bool turnOnHighlighted();
		bool turnOffHighlighted();

	public:

		HexTState(const ColorId pId, 
				  const int pRingIndex, 
				  const int pTileIndex) :HexTile(pId, pRingIndex, pTileIndex) { }

		bool stateOnDeactivate();
		bool stateOnActivate(TileState);
		void setState(TileState) override;
	};
}