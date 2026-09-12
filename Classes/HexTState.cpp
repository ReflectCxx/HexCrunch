
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
		case TileState::Stray: return turnOffStray();
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
		case TileState::Stray: return turnOnStray();
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


	bool HexTState::turnOnStray()
	{
		getBackground().setVisible(true);
		const auto bg = getBackground().getChildren().at(0);
		bg->setOpacity(255 * 0.3f);
		
		return true;
	}


	bool HexTState::turnOffStray()
	{
		getBackground().setVisible(false);
		const auto bg = getBackground().getChildren().at(0);
		bg->setOpacity(255);
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
		getBackground().setScale(1.f);
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
		getBackground().setScale(1.1f);
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
		getBackground().setScale(1.1f);

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
		getBackground().setScale(1.f);
		return true;
	}
}