
#include "Asset.h"
#include "AssetConsts.h"
#include "DrawingUtils.h"

USING_NS_CC;


namespace
{
	inline const Color4F col4f(const hex::ColorId pId)
	{
		switch (pId) {
		case hex::ColorId::Red: return HEXCOL_RED;
		case hex::ColorId::Blue: return HEXCOL_BLUE;
		case hex::ColorId::Green: return HEXCOL_GREEN; //*/HEXCOL_TEAL_GREEN;
		case hex::ColorId::Yellow: return HEXCOL_YELLOW;
		case hex::ColorId::Purple: return HEXCOL_PURPLE;
		default: return HEXCOL_BIEGE;
		}
	}


	//sprite used for rendering regular hexagon tile.
	inline const std::string hexNormal(const hex::ColorId pColor)
	{
		switch (pColor) {
		case hex::ColorId::Red: return hex::TILE_RED_BG;
		case hex::ColorId::Blue: return hex::TILE_BLUE_BG;
		case hex::ColorId::Green: return hex::TILE_GREEN_BG;
		case hex::ColorId::Yellow: return hex::TILE_YELLOW_BG;
		case hex::ColorId::Purple: return hex::TILE_PURPLE_BG;
		default: return hex::TILE_BIEGE;
		}
	}

	//sprite used for rendering hexagon tile as blocked.
	inline const std::string hexBlocked(const hex::ColorId pColor)
	{
		switch (pColor) {
		case hex::ColorId::Red: return hex::TILE_RED;
		case hex::ColorId::Blue: return hex::TILE_BLUE;
		case hex::ColorId::Green: return hex::TILE_GREEN;
		case hex::ColorId::Yellow: return hex::TILE_YELLOW;
		case hex::ColorId::Purple: return hex::TILE_PURPLE;
		default: return hex::TILE_BIEGE;
		}
	}
}



namespace hex
{
	cocos2d::Sprite* Asset::createTileBg(const Size& pSz)
	{
		return createTile(TILE_BG, pSz);
	}


	Sprite* Asset::createNormalTile(const ColorId pColor, const Size& pSz)
	{
		return createTile(hexNormal(pColor), pSz);
	}


	Sprite* Asset::createBlockedTile(const ColorId pColor, const Size& pSz)
	{
		return createTile(hexBlocked(pColor), pSz);
	}


	void Asset::drawHexLink(DrawNode* pNode, const ColorId pColor,
							const Vec2& pOrigin, const Size& pSz,
						    const float pAngle, bool pClear) {
		if (pClear) {
			pNode->clear();
		}
		ut::draw_capsule(pNode, pSz, col4f(pColor), pOrigin, pAngle);
	}


	Sprite* Asset::createTile(const std::string& pName, const Size& pSz)
	{
		auto tile = Sprite::create(pName);
		const auto& sz = tile->getContentSize();
		const auto scaleX = pSz.width / sz.width;
		const auto scaleY = pSz.height / sz.height;
		tile->setScale(scaleX, scaleY);
		return tile;
	}


	Sprite* Asset::createGameBg()
	{
		auto bg = Sprite::create(GAME_BG);
		const auto scaleX = SCR_WIDTH / bg->getContentSize().width;
		const auto scaleY = SCR_HEIGHT / bg->getContentSize().height;
		bg->setScale(scaleX, scaleY);
		bg->setPosition({ SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f });
		bg->setOpacity(255 * 0.95f);
		return bg;
	}


	Menu* Asset::createExitBtn(std::function<void(Ref*)> pCb)
	{
		auto closeItem = MenuItemImage::create(BTN_EXIT_NORMAL, BTN_EXIT_SELECTED, pCb);
		const auto x = (SCR_WIDTH - closeItem->getContentSize().width / 2.f);
		const auto y = (closeItem->getContentSize().height / 2.f);
		closeItem->setPosition({ x, y });
		auto menu = Menu::create(closeItem, nullptr);
		menu->setPosition(Vec2::ZERO);
		return menu;
	}
}