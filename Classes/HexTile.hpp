#pragma once

#include "HexTile.h"

namespace hex
{	
	constexpr void HexTile::assignColor(const ColorId pColor) {
		m_colorId = pColor;
	}

	inline const float HexTile::getArrowAngle() {
		return (getHexRingEdgeAngle() - 60.f);
	}

	constexpr cocos2d::Node& HexTile::getArrow() {
		return *m_arrow;
	}

	constexpr cocos2d::Node& HexTile::getRingFace() {
		return *m_ringFace;
	}

	constexpr cocos2d::Node& HexTile::getIdleFace() {
		return *m_hexIdle;
	}

	constexpr cocos2d::Node& HexTile::getClippedFace() {
		return *m_hexClipped;
	}

	constexpr cocos2d::Node& HexTile::getBackground() {
		return *m_background;
	}

	constexpr cocos2d::Node& HexTile::getBlockedFace() {
		return *m_hexBlocked;
	}

	constexpr cocos2d::DrawNode& HexTile::getLink() {
		return *m_hexLink;
	}
}