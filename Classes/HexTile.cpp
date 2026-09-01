

#include "HexTileUtils.h"
#include "HexTileState.h"
#include "DrawingUtils.h"

USING_NS_CC;

namespace
{
	static constexpr auto Z_TILE_COLOR = 0;
	static constexpr auto Z_TILE_CLIPPED = 1;
	static constexpr auto Z_TILE_HIGHLIGHT = 2;
	static constexpr auto COLOR_SPRITE_TAG = 99;
}


namespace hex
{
	HexTile::HexTile(const int pRingIndex, const int pTileIndex)
		: Hex(pRingIndex, pTileIndex)
		, m_clipped(nullptr)
		, m_highlight(nullptr)
		, m_background(nullptr)
		, m_foreground(nullptr)
	{ }


	HexTile* HexTile::create(const ColorId pId, const int pRingIndex, const int pTileIndex)
	{
		auto pRet = new(std::nothrow) HexTile(pRingIndex, pTileIndex);
		if (pRet && pRet->init(pId, pRingIndex, pTileIndex)) {
			pRet->autorelease();
		}
		else {
			delete pRet;
			pRet = nullptr;
		}
		return pRet;
	}


	bool HexTile::init(const ColorId pId, const int pRingIndex, const int pTileIndex)
	{
		if (!Node::init()) {
			return false;
		}
		setColorId(pId);

		constexpr auto radius = (HEX_RAD - HEX_BORDER);
		const auto sz = Size{ SQRT_3 * radius, 2.f * radius };

		m_foreground = Node::create();
		m_foreground->setContentSize(sz);
		addChild(m_foreground, Z_TILE_COLOR);

		m_highlight = Node::create();
		addChild(m_highlight, Z_TILE_HIGHLIGHT);

		const auto create = [&sz](const std::string pStr, float scale = 1.f) {
			auto tile = Sprite::create(pStr);
			const auto& ssz = tile->getContentSize();
			const auto scaleX = (sz.width * scale)/ ssz.width;
			const auto scaleY = (sz.height * scale)/ ssz.height;
			tile->setScale(scaleX, scaleY);
			return tile;
		};
		//m_hexHighlight->addChild(create(TILE_HIGH));
		refreshTileColor();
		addIndexLabel();
		return true;
	}
}


 
namespace hex
{
	void HexTile::refreshTileColor()
	{
		const auto& sz = getForeground().getContentSize();
		const auto color = getColorId();
		{
			auto tile = HexTileUtils::tileSprite(sz, color);
			getForeground().removeChildByTag(COLOR_SPRITE_TAG);
			getForeground().addChild(tile, 0, COLOR_SPRITE_TAG);
		}

		if (m_clipped != nullptr)
		{
			auto tile = HexTileUtils::tileSprite(sz, color);
			getClipped().removeAllChildren();
			getClipped().addChild(tile);
		}

		if (m_pathLink.node != nullptr) {
			HexTileUtils::drawLinkCapsule(m_pathLink, color, { LINK_WIDTH, LINK_HEIGHT });
		}
	}


	void HexTile::setRingPathLink(const PathLink& pLink)
	{
		const auto tileSz = getForeground().getContentSize();
		const auto clipSz = Size{ CLIP_WIDTH, CLIP_HEIGHT };

		m_clipped = Node::create();
		auto tile = HexTileUtils::tileSprite(tileSz, getColorId());
		getClipped().addChild(tile);

		auto capsule = DrawNode::create();
		auto clipped = HexTileUtils::createClipped(m_clipped, capsule);
		HexTileUtils::drawLinkCapsule({ pLink.angle, pLink.origin, capsule }, getColorId(), clipSz);
		addChild(clipped, Z_TILE_CLIPPED);

		m_pathLink = pLink;
		getLink().setVisible(false);
		getClipped().setVisible(false);

		const auto num = getTileIndex() / (getRingIndex() + 1);
		const auto theta = -60.f * (1.f + float(num));
		getClipped().setRotation(theta);
		getForeground().setRotation(theta);
	}


	void HexTile::setState(TileState pState)
	{
		if (pState == getCurrentState()) {
			return;
		}
		if (pState != TileState::kIdle) {
			setState(TileState::kIdle);
		}

		setCurrentState(pState);
		switch (getCurrentState())
		{
		case TileState::kIdle: {
			HexTileState::onIdle(*this);
			return;
		}
		case TileState::kClipped:
		{
			HexTileState::onClipped(*this);
			return;
		}
		case TileState::kSelected:
		{
			HexTileState::onSelected(*this);
			return;
		}
		case TileState::kHighlighted:
		{
			HexTileState::onHighlighted(*this);
			return;
		}
		default: return;
		}
	}
}



namespace hex
{
	void HexTile::addIndexLabel()
	{
		const auto ri = getRingIndex();
		const auto ti = getTileIndex();
		std::string str;// = std::to_string(ri) + ", ";
		str += std::to_string(ti);
		auto label = Label::createWithTTF(str, FONT, 50.f);
		//addChild(label, 3);
	}
}