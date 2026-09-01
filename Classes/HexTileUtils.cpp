
#include "Constants.h"
#include "HexTileUtils.h"
#include "DrawingUtils.h"

USING_NS_CC;

namespace hex
{
	const std::string HexTileUtils::toStr(const ColorId pId)
	{
		switch (pId) {
		case ColorId::kRed: return TILE_RED;
		case ColorId::kBlue: return TILE_BLUE;
		case ColorId::kGreen: return TILE_GREEN;
		case ColorId::kYellow: return TILE_YELLOW;
		case ColorId::kPurple: return TILE_PURPLE;
		default:break;
		}
		return TILE_BIEGE;
	}


	const std::string HexTileUtils::bgStr(const ColorId pId)
	{
		switch (pId) {
		case ColorId::kRed: return TILE_RED_BG;
		case ColorId::kBlue: return TILE_BLUE_BG;
		case ColorId::kGreen: return TILE_GREEN_BG;
		case ColorId::kYellow: return TILE_YELLOW_BG;
		case ColorId::kPurple: return TILE_PURPLE_BG;
		default:break;
		}
		return TILE_BIEGE;
	}


	const cocos2d::Color4F HexTileUtils::toColor(const ColorId pId)
	{
		switch (pId) {
		case ColorId::kRed: return HEXCOL_RED;
		case ColorId::kBlue: return HEXCOL_BLUE;
		case ColorId::kGreen: return HEXCOL_TEAL_GREEN;
		case ColorId::kYellow: return HEXCOL_YELLOW;
		case ColorId::kPurple: return HEXCOL_PURPLE;
		default:break;
		}
		return cocos2d::Color4F::ORANGE;
	}
}


namespace hex
{
	void HexTileUtils::drawLinkCapsule(const PathLink& pLink, ColorId pColor, const cocos2d::Size& pSize, bool pClear)
	{
		if (pClear) {
			pLink.node->clear();
		}
		const auto color = toColor(pColor);
		ut::draw_capsule(pLink.node, pSize, color, pLink.origin, pLink.angle);
	}

	void HexTileUtils::swapTileColor(HexTile* pTileA, HexTile* pTileB)
	{
		std::swap(pTileA->m_tileId, pTileB->m_tileId);
		pTileA->refreshTileColor();
		pTileB->refreshTileColor();
	}

	 
	Node* HexTileUtils::tileSprite(const Size& pSz, const std::string& pName, const float pScale)
	{
		auto tile = Sprite::create(pName);
		const auto& sz = tile->getContentSize();
		const auto scaleX = (pSz.width * pScale) / sz.width;
		const auto scaleY = (pSz.height * pScale) / sz.height;
		tile->setScale(scaleX, scaleY);
		return tile;
	}


	ClippingNode* HexTileUtils::createClipped(cocos2d::Node* pNode, cocos2d::Node* pClipper)
	{
		auto clippingNode = ClippingNode::create();
		clippingNode->setStencil(pClipper);
		clippingNode->setInverted(true);
		clippingNode->setAlphaThreshold(0.05f);
		clippingNode->addChild(pNode);
		return clippingNode;
	}
}