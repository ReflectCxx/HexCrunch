#pragma once

#include "HexTile.h"

namespace hex
{
	inline constexpr bool HexTile::isBgEnabled() const {
		return m_background->getParent()->isVisible();
	}


	inline void HexTile::setState(TileState pState) {
		switchToState(pState);
	}

	inline constexpr cocos2d::Node& HexTile::getRingFace() {
		return *m_ringFace;
	}

	inline constexpr cocos2d::Node& HexTile::getForeground() {
		return *m_foreground;
	}

	inline constexpr cocos2d::Node& HexTile::getBackground() {
		return *m_background;
	}

	inline constexpr cocos2d::Node& HexTile::getBlocked() {
		return *m_blocked;
	}

	inline constexpr cocos2d::DrawNode& HexTile::getLink() {
		return *m_hexLink;
	}
}