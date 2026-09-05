
#include "HexTile.hpp"
#include "HexTState.h"
#include "DrawingUtils.h"

USING_NS_CC;


namespace hex
{
	bool HexTState::initStateZero()
	{
		getLink().setVisible(false);
		getRingFace().setVisible(false);
		getBlocked().setVisible(false);
		getBackground().setVisible(false);
		getForeground().setVisible(false);
		return true;
	}


	bool HexTState::stateOnDeactivate()
	{
		switch (getCurrentState()) {
		case TileState::None:return initStateZero();
		case TileState::Idle: return turnOffIdle();
		case TileState::Actor: return turnOffActing();
		case TileState::RingFace: return turnOffClipped();
		case TileState::Blocked: return turnOffBlocked();
		case TileState::Highlighted: return turnOffHighlighted();
		default: return false;
		}
	}


	bool HexTState::stateOnActivate(TileState pState)
	{
		if (pState == getCurrentState()) {
			return false;
		}
		switch (pState) {
		case TileState::Idle: return turnOnIdle();
		case TileState::Actor: return turnOnActing();
		case TileState::RingFace: return turnOnClipped();
		case TileState::Blocked: return turnOnBlocked();
		case TileState::Highlighted: return turnOnHighlighted();
		default: return false;
		}
	}
}


namespace hex
{
	bool HexTState::turnOffIdle() {
		getForeground().setVisible(false);
		return true;
	}


	bool HexTState::turnOnIdle() {
		getForeground().setVisible(true);
		return true;
	}
}


namespace hex
{
	bool HexTState::turnOffBlocked() {
		getBlocked().setVisible(false);
		return true;
	}


	bool HexTState::turnOnBlocked()
	{
		if (getPreviousState() == TileState::RingFace) {
			getLink().setVisible(true);
		}
		getBlocked().setVisible(true);
		return true;
	}
}


namespace hex
{
	bool HexTState::turnOffClipped()
	{
		getLink().setVisible(false);
		getRingFace().setVisible(false);
		return true;
	}


	bool HexTState::turnOnClipped()
	{
		getLink().setVisible(true);
		getRingFace().setVisible(true);
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

		getLink().setVisible(false);
		getRingFace().setVisible(false);
		getBackground().setVisible(false);
		return true;
	}


	bool HexTState::turnOnActing()
	{
		constexpr auto radius = HEX_BORDER + (HEX_RAD * (1 - BOUNCE_SCALE));
		ut::run_hex_bounce(&getClippedFace(), radius, nullptr);
		getClippedFace().setScale(BOUNCE_SCALE);

		getLink().setVisible(true);
		getRingFace().setVisible(true);
		getBackground().setVisible(true);
		return true;
	}
}


namespace hex 
{
	bool HexTState::turnOnHighlighted()
	{
		constexpr auto radius = HEX_BORDER + (HEX_RAD * (1 - BOUNCE_SCALE));
		ut::run_hex_bounce(&getClippedFace(), radius, [&](float theta) {
			getClippedFace().setRotation(theta);
		});
		getClippedFace().setScale(BOUNCE_SCALE);

		getLink().setVisible(true);
		const auto pt = static_cast<HexTState*>(getPrevoiusRingTile());
		pt->getLink().setVisible(true);

		getRingFace().setVisible(true);
		getBackground().setVisible(true);
		return true;
	}


	bool HexTState::turnOffHighlighted()
	{
		getClippedFace().setScale(1.f);
		getClippedFace().unscheduleAllCallbacks();
		getClippedFace().setPosition(Vec2::ZERO);
		getClippedFace().setRotation(getHexRingEdgeAngle());

		getLink().setVisible(false);
		const auto pt = static_cast<HexTState*>(getPrevoiusRingTile());
		pt->getLink().setVisible(false);

		getRingFace().setVisible(false);
		getBackground().setVisible(false);
		return true;
	}
}