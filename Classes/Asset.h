#pragma once

#include <functional>

#include "cocos2d.h"
#include "AssetConsts.h"
#include "Constants.h"

namespace hex
{
	class Asset
	{
		static cocos2d::Sprite* createTile(const std::string&, const cocos2d::Size&);

	public:

		static cocos2d::Sprite* createGameBg();
		static cocos2d::Sprite* createNormalTile(const ColorId, const cocos2d::Size&);
		static cocos2d::Sprite* createBlockedTile(const ColorId, const cocos2d::Size&);

		static cocos2d::Menu* createExitBtn(std::function<void(cocos2d::Ref*)> pCb);		

		static void drawHexLink(cocos2d::DrawNode* pNode, const ColorId pColor,
							    const cocos2d::Vec2& pOrigin, const cocos2d::Size& pSz,
							    const float pAngle, bool pClear = true);
	};
}