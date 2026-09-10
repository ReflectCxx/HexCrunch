#pragma once

#include "HexTile.h"

namespace hex
{	
	constexpr void HexTile::assignColor(const ColorId pColor) {
		m_colorId = pColor;
	}

	constexpr cocos2d::Node& HexTile::getRingFace() {
		return *m_ringFace;
	}

	constexpr cocos2d::Node& HexTile::getIdleFace() {
		return *m_idleFace;
	}

	constexpr cocos2d::Node& HexTile::getClippedFace() {
		return *m_clipped;
	}

	constexpr cocos2d::Node& HexTile::getBackground() {
		return *m_background;
	}

	constexpr cocos2d::Node& HexTile::getBlockedFace() {
		return *m_blockFace;
	}

	constexpr cocos2d::DrawNode& HexTile::getLink() {
		return *m_hexLink;
	}
}