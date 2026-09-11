#pragma once

#include "HexTile.hpp"
#include "StateMachine.h"

namespace hex
{
	class HexTState : public HexTile, public StateMachine<TileState, HexTState>
	{
		void initStateZero();

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

		bool turnOnStray();
		bool turnOffStray();

	public:

		HexTState(const ColorId pId, 
				  const int pRingIndex, 
				  const int pTileIndex) :HexTile(pId, pRingIndex, pTileIndex) { }

		bool stateOnDeactivate();
		bool stateOnActivate(TileState);
		void setState(const TileState) override;
		const TileState getState() override;
	};
}