
#include "HexTile.h"
#include "HexTileState.h"
#include "HexTileUtils.h"
#include "DrawingUtils.h"

USING_NS_CC;

namespace hex
{
	void HexTileState::setToClipped(HexTile& pTile)
	{
		pTile.getLink().setVisible(true);
		pTile.getClipped().setVisible(true);
		pTile.getForeground().setVisible(false);
		pTile.getBackground().setVisible(false);
		pTile.getHighlight().setVisible(false);
	}


	void HexTileState::setToActing(HexTile& pTile)
	{
		constexpr auto radius = HEX_BORDER + (HEX_RAD * (1 - BOUNCE_SCALE));
		ut::run_hex_bounce(&pTile.getClipped(), radius, nullptr);
		pTile.getClipped().setScale(BOUNCE_SCALE);

		pTile.getLink().setVisible(true);
		pTile.getClipped().setVisible(true);
		pTile.getBackground().setVisible(true);
		pTile.getForeground().setVisible(false);
	}


	void HexTileState::setToHighlighted(HexTile& pTile)
	{
		constexpr auto radius = HEX_BORDER + (HEX_RAD * (1 - BOUNCE_SCALE));
		ut::run_hex_bounce(&pTile.getForeground(), radius, nullptr);
		pTile.getForeground().setScale(BOUNCE_SCALE);

		pTile.getHighlight().clear();
		ut::draw_hexagon(&pTile.getHighlight(), HEX_RAD, Color4F::WHITE, CORNER_RAD);

		pTile.getLink().setVisible(false);
		pTile.getClipped().setVisible(false);
		pTile.getForeground().setVisible(true);
		pTile.getBackground().setVisible(false);
		pTile.getHighlight().setVisible(true);
	}


	void HexTileState::setToIdle(HexTile& pTile)
	{
		pTile.getLink().setVisible(false);
		pTile.getClipped().setVisible(false);
		pTile.getBackground().setVisible(false);
		pTile.getForeground().setVisible(true);
		pTile.getHighlight().setVisible(false);

		const auto reset = [](Node& node) {
			node.setScale(1.f);
			node.unscheduleAllCallbacks();
			node.setPosition(Vec2::ZERO);
		};
		reset(pTile.getClipped());
		reset(pTile.getForeground());
		pTile.getHighlight().clear();
	}
}