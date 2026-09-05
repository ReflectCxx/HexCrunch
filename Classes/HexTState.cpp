
#include "HexTile.hpp"
#include "HexTState.h"
#include "DrawingUtils.h"
#include "HexTState.h"

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
	bool HexTState::turnOffIdle()
	{
		getForeground().setVisible(false);
		return true;
	}


	bool HexTState::turnOnIdle()
	{
		getForeground().setVisible(true);
		return true;
	}


	bool HexTState::turnOffBlocked()
	{
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


	bool HexTState::turnOffActing()
	{
		getRingFace().setScale(1.f);
		getRingFace().unscheduleAllCallbacks();
		getRingFace().setPosition(Vec2::ZERO);

		getLink().setVisible(false);
		getRingFace().setVisible(false);
		getBackground().setVisible(false);
		return true;
	}


	bool HexTState::turnOnActing()
	{
		constexpr auto radius = HEX_BORDER + (HEX_RAD * (1 - BOUNCE_SCALE));
		ut::run_hex_bounce(&getRingFace(), radius, nullptr);
		getRingFace().setScale(BOUNCE_SCALE);

		getLink().setVisible(true);
		getRingFace().setVisible(true);
		getBackground().setVisible(true);
		return true;
	}


	bool HexTState::turnOffHighlighted()
	{
		getRingFace().setScale(1.f);
		getRingFace().unscheduleAllCallbacks();
		getRingFace().setPosition(Vec2::ZERO);
		getRingFace().setRotation(getHexRingEdgeAngle());

		getLink().setVisible(false);
		getRingFace().setVisible(false);
		if (getRingIndex() != RING_COUNT) {
			getBackground().setVisible(false);
			if (isBgEnabled()) {
				const auto pt = static_cast<HexTState*>(getPrevoiusRingTile());
				pt->getLink().setVisible(false);
			}
		}
		return true;
	}

	HexTState::HexTState(const ColorId pId, const int pRingIndex, const int pTileIndex)
		: HexTile(pId, pRingIndex, pTileIndex)
	{
	}


	bool HexTState::turnOnHighlighted()
	{
		constexpr auto radius = HEX_BORDER + (HEX_RAD * (1 - BOUNCE_SCALE));
		ut::run_hex_bounce(&getRingFace(), radius, [&](float theta) {
			getRingFace().setRotation(theta);
		});
		getRingFace().setScale(BOUNCE_SCALE);

		getLink().setVisible(true);
		getRingFace().setVisible(true);
		if (getRingIndex() != RING_COUNT) {
			getBackground().setVisible(true);
			if (isBgEnabled()) {
				const auto pt = static_cast<HexTState*>(getPrevoiusRingTile());
				pt->getLink().setVisible(true);
			}
		}
		return true;
	}
}