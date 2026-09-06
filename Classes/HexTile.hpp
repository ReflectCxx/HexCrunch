#pragma once

#include "HexTile.h"

namespace hex
{
	inline constexpr cocos2d::Node& HexTile::getRingFace() {
		return *m_ringFace;
	}

	inline constexpr cocos2d::Node& HexTile::getForeground() {
		return *m_foreground;
	}

	inline constexpr cocos2d::Node& HexTile::getClippedFace()
	{
		return *m_clipped;
	}

	inline constexpr cocos2d::Node& HexTile::getBackground() {
		return *m_background;
	}

	inline constexpr cocos2d::Node& HexTile::getBlockedFace() {
		return *m_blocked;
	}

	inline constexpr cocos2d::DrawNode& HexTile::getLink() {
		return *m_hexLink;
	}
}