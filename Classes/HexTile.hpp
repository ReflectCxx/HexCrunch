#pragma once

#include "HexTile.h"

namespace hex
{	
	constexpr cocos2d::Node& HexTile::getRingFace() {
		return *m_ringFace;
	}

	constexpr cocos2d::Node& HexTile::getForeground() {
		return *m_foreground;
	}

	constexpr cocos2d::Node& HexTile::getClippedFace() {
		return *m_clipped;
	}

	constexpr cocos2d::Node& HexTile::getBackground() {
		return *m_background;
	}

	constexpr cocos2d::Node& HexTile::getBlockedFace() {
		return *m_blocked;
	}

	constexpr cocos2d::DrawNode& HexTile::getLink() {
		return *m_hexLink;
	}

	constexpr void HexTile::updateViewColor(const ColorId pColor)
	{
		if (pColor == m_colorId) {
			return;
		}
		if (pColor != ColorId::None) {
			m_colorId = pColor;
			refreshView();
			return;
		}
		m_colorId = pColor;
	}
}