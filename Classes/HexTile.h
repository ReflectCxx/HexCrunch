#pragma once

#include "cocos2d.h"

#include "HexT.hpp"

namespace hex
{
	class HexTile : public Hex<HexTile>, public cocos2d::Node
	{
		cocos2d::Node* m_arrow;
		cocos2d::Node* m_hexIdle;
		cocos2d::Node* m_hexBlocked;
		cocos2d::Node* m_hexClipped;
		cocos2d::DrawNode* m_hexLink;

		cocos2d::ClippingNode* m_ringFace;
		cocos2d::ClippingNode* m_background;

		void initClippedBg(cocos2d::Node* pGridNode);
		bool init(const ColorId pId, const int pRingIndex, const int pTileIndex);

	protected:

		HexTile(const ColorId pId, const int pRingIndex, const int pTileIndex);

		constexpr cocos2d::Node& getBackground();
		constexpr cocos2d::Node& getBlockedFace();

	public:

		void refreshView();
		void swapColor(HexTile&, bool pRefreshView);
		void initRingPlacement(cocos2d::Node* pGridNode, cocos2d::Node* pLinkNode);

		static HexTile* create(const ColorId pId, const int pRingIndex, const int pTileIndex);

		const float getArrowAngle();
		constexpr cocos2d::Node& getArrow();
		constexpr cocos2d::Node& getRingFace();
		constexpr cocos2d::Node& getIdleFace();
		constexpr cocos2d::Node& getClippedFace();
		constexpr cocos2d::DrawNode& getLink();
		constexpr void assignColor(const ColorId pColor);

		virtual void setState(const TileState) = 0;
		virtual const TileState getState() = 0;
	};
}