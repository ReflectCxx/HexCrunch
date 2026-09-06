#pragma once

#include "cocos2d.h"

#include "HexT.hpp"

namespace hex
{
	class HexTile : public Hex<HexTile>, public cocos2d::Node
	{
		cocos2d::Node* m_blocked;
		cocos2d::Node* m_clipped;
		cocos2d::Node* m_foreground;
		cocos2d::Node* m_clippedBg;
		cocos2d::DrawNode* m_hexLink;
		cocos2d::ClippingNode* m_ringFace;
		cocos2d::ClippingNode* m_background;

		void refreshView();
		void initClippedBg(cocos2d::Node* pGridNode);
		bool init(const ColorId pId, const int pRingIndex, const int pTileIndex);

	protected:

		HexTile(const ColorId pId, const int pRingIndex, const int pTileIndex);

		constexpr cocos2d::Node& getBackground();
		constexpr cocos2d::Node& getBlockedFace();
		constexpr cocos2d::DrawNode& getLink();

	public:

		void swapColors(HexTile&);
		void initRingPlacement(cocos2d::Node* pGridNode, cocos2d::Node* pLinkNode);

		static HexTile* create(const ColorId pId, const int pRingIndex, const int pTileIndex);

		constexpr cocos2d::Node& getRingFace();
		constexpr cocos2d::Node& getForeground();
		constexpr cocos2d::Node& getClippedFace();

		virtual void setState(TileState) = 0;
	};
}