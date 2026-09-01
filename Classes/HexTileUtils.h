#pragma once

#include "cocos2d.h"

#include "HexTile.h"

namespace hex
{
	struct HexTileUtils
	{
		static const std::string toStr(const ColorId pId);

		static const std::string bgStr(const ColorId pId);

		static const cocos2d::Color4F toColor(const ColorId pId);

		static void swapTileColor(HexTile* pTileA, HexTile* pTileB);

		static cocos2d::ClippingNode* createClipped(cocos2d::Node* pNode,
													cocos2d::Node* pClip);

		static cocos2d::Node* tileSprite(const cocos2d::Size& pSz,
										 const std::string& pName, const float pScale = 1.f);

		static void drawLinkCapsule(const PathLink& pLink, ColorId pColor,
									const cocos2d::Size& pSize, bool pClear = true);
	};
}