#pragma once

#include "HexTile.h"

namespace hex
{	
	constexpr void HexTile::assignColor(const ColorId pColor) {
		m_meta.color = pColor;
	}

	inline const float HexTile::getArrowAngle() const {
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

	constexpr void HexTile::setSpawnColor(ColorId pColor) {
		m_meta.spawnColor = pColor;
	}

	constexpr ColorId HexTile::getColorId() const {
		return m_meta.color;
	}

	constexpr ColorId HexTile::getSpawnColor() const {
		return m_meta.spawnColor;
	}

	constexpr float HexTile::getHexRingEdgeAngle() const
	{
		const auto num = (getTileIndex() / (getRingIndex() + 1));
		const auto theta = (-60.f * (1.f + float(num)));
		return theta;
	}
}