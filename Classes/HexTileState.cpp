
#include "HexTile.h"
#include "HexTileState.h"
#include "HexTileUtils.h"
#include "DrawingUtils.h"

USING_NS_CC;

namespace hex
{
	bool HexTileState::initNoState(HexTile& pTile)
	{
		pTile.getLink().setVisible(false);
		pTile.getClipped().setVisible(false);
		pTile.getHighlight().setVisible(false);
		pTile.getBackground().setVisible(false);
		return true;
	}
}



namespace hex
{
	bool HexTileState::turnOffIdle(HexTile& pTile)
	{
		pTile.getForeground().setVisible(false);
		return true;
	}


	bool HexTileState::turnOnIdle(HexTile& pTile)
	{
		pTile.getForeground().setVisible(true);
		return true;
	}


	bool HexTileState::turnOffClipped(HexTile& pTile)
	{
		pTile.getLink().setVisible(false);
		pTile.getClipped().setVisible(false);
		return true;
	}


	bool HexTileState::turnOnClipped(HexTile& pTile)
	{
		pTile.getLink().setVisible(true);
		pTile.getClipped().setVisible(true);
		return true;
	}


	bool HexTileState::turnOffActing(HexTile& pTile)
	{
		pTile.getClipped().setScale(1.f);
		pTile.getClipped().unscheduleAllCallbacks();
		pTile.getClipped().setPosition(Vec2::ZERO);

		pTile.getLink().setVisible(false);
		pTile.getClipped().setVisible(false);
		pTile.getBackground().setVisible(false);
		return true;
	}


	bool HexTileState::turnOnActing(HexTile& pTile)
	{
		constexpr auto radius = HEX_BORDER + (HEX_RAD * (1 - BOUNCE_SCALE));
		ut::run_hex_bounce(&pTile.getClipped(), radius, nullptr);
		pTile.getClipped().setScale(BOUNCE_SCALE);

		pTile.getLink().setVisible(true);
		pTile.getClipped().setVisible(true);
		pTile.getBackground().setVisible(true);
		return true;
	}


	bool HexTileState::turnOffHighlighted(HexTile& pTile)
	{
		pTile.getClipped().setScale(1.f);
		pTile.getClipped().unscheduleAllCallbacks();
		pTile.getClipped().setPosition(Vec2::ZERO);

		pTile.getLink().setVisible(false);
		pTile.getClipped().setVisible(false);
		if (pTile.getRingIndex() != RING_COUNT) {
			pTile.getBackground().setVisible(false);
		}

		auto& previousTile = *(static_cast<HexTile*>(pTile.getPrevoiusRingTile()));
		if (previousTile.m_pathLink.node != nullptr) {
			previousTile.getLink().setVisible(false);
		}
		return true;
	}


	bool HexTileState::turnOnHighlighted(HexTile& pTile)
	{
		constexpr auto radius = HEX_BORDER + (HEX_RAD * (1 - BOUNCE_SCALE));
		ut::run_hex_bounce(&pTile.getClipped(), radius, nullptr);
		pTile.getClipped().setScale(BOUNCE_SCALE);

		pTile.getLink().setVisible(true);
		pTile.getClipped().setVisible(true);
		if (pTile.getRingIndex() != RING_COUNT) {
			pTile.getBackground().setVisible(true);
		}

		auto& previousTile = *(static_cast<HexTile*>(pTile.getPrevoiusRingTile()));
		if (previousTile.m_pathLink.node != nullptr) {
			previousTile.getLink().setVisible(true);
		}
		return true;
	}
}