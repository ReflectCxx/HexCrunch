
#include "Game.h"
#include "HexGrid.h"
#include "HexTile.hpp"
#include "HexTState.h"
#include "DrawingUtils.h"

USING_NS_CC;


namespace hex
{
	void HexTState::setState(TileState pState) {
		switchToState(pState);
	}

	const TileState HexTState::getState() {
		return getCurrentState();
	}


	void HexTState::initStateZero()
	{
		refreshView();
		getLink().setVisible(false);
		getBackground().setVisible(false);

		getRingFace().setVisible(false);
		getIdleFace().setVisible(false);
		getBlockedFace().setVisible(false);
	}


	bool HexTState::stateOnDeactivate()
	{
		switch (getCurrentState()) {
		case TileState::Idle: return turnOffIdle();
		case TileState::Actor: return turnOffActing();
		case TileState::Follower: return turnOffFollower();
		case TileState::RingFace: return turnOffRingFace();
		case TileState::Blocked: return turnOffBlocked();
		case TileState::Highlighted: return turnOffHighlighted();
		case TileState::None: {
			initStateZero();
			return true;
		}	
		default: return false;
		}
	}


	bool HexTState::stateOnActivate(TileState pState)
	{
		switch (pState) {
		case TileState::None: return true;
		case TileState::Idle: return turnOnIdle();
		case TileState::Actor: return turnOnActing();
		case TileState::Follower: return turnOnFollower();
		case TileState::RingFace: return turnOnRingFace();
		case TileState::Blocked: return turnOnBlocked();
		case TileState::Highlighted: return turnOnHighlighted();
		default: return false;
		}
	}
}


namespace hex
{
	bool HexTState::turnOffIdle() {
		getIdleFace().setVisible(false);
		return true;
	}


	bool HexTState::turnOnIdle() {
		getIdleFace().setVisible(true);
		return true;
	}
}


namespace hex
{
	bool HexTState::turnOffBlocked() {
		getBlockedFace().setVisible(false);
		return true;
	}


	bool HexTState::turnOnBlocked()
	{
		if (getPreviousState() == TileState::RingFace) {
			getLink().setVisible(true);
		}
		getBlockedFace().setVisible(true);
		return true;
	}
}


namespace hex
{
	bool HexTState::turnOffRingFace()
	{
		getLink().setVisible(false);
		getRingFace().setVisible(false);
		return true;
	}


	bool HexTState::turnOnRingFace()
	{
		getLink().setVisible(true);
		getRingFace().setVisible(true);
		return true;
	}


	bool HexTState::turnOnHighlighted()
	{
		//const auto ang = getIdleFace().getRotation();
		//getIdleFace().setRotation(ang + 60.f);
		getIdleFace().setVisible(true);
		return true;
	}


	bool HexTState::turnOffHighlighted()
	{
		//const auto ang = getIdleFace().getRotation();
		//getIdleFace().setRotation(ang - 60.f);
		getIdleFace().setVisible(false);
		return true;
	}
}


namespace hex
{
	bool HexTState::turnOffActing()
	{
		getClippedFace().setScale(1.f);
		getClippedFace().unscheduleAllCallbacks();
		getClippedFace().setPosition(Vec2::ZERO);
		getClippedFace().setRotation(getHexRingEdgeAngle());

		getLink().setVisible(false);
		getRingFace().setVisible(false);
		getBackground().setVisible(false);
		return true;
	}


	bool HexTState::turnOnActing()
	{
		constexpr auto radius = HEX_BORDER + (HEX_RAD * (1 - BOUNCE_SCALE));
		ut::run_hex_bounce(&getClippedFace(), radius, [&](float theta) {
			getClippedFace().setRotation(theta);
		});
		getClippedFace().setScale(BOUNCE_SCALE);

		getLink().setVisible(true);
		getRingFace().setVisible(true);
		getBackground().setVisible(true);
		return true;
	}
}


namespace hex 
{
	bool HexTState::turnOnFollower()
	{
		retain();
		removeFromParentAndCleanup(false);
		Game::instance().grid().getHexBGNode().addChild(this);
		release();

		constexpr auto radius = HEX_BORDER + (HEX_RAD * (1 - BOUNCE_SCALE));
		ut::run_hex_bounce(&getClippedFace(), radius, [&](float theta) {
			getClippedFace().setRotation(theta);
		});
		getClippedFace().setScale(BOUNCE_SCALE);

		getLink().setVisible(true);
		getRingFace().setVisible(true);
		getBackground().setVisible(true);

		return true;
	}


	bool HexTState::turnOffFollower()
	{
		retain();
		removeFromParentAndCleanup(false);
		Game::instance().grid().getHexNode().addChild(this);
		release();

		getClippedFace().setScale(1.f);
		getClippedFace().unscheduleAllCallbacks();
		getClippedFace().setPosition(Vec2::ZERO);
		getClippedFace().setRotation(getHexRingEdgeAngle());

		getLink().setVisible(false);
		getRingFace().setVisible(false);
		getBackground().setVisible(false);
		return true;
	}
}