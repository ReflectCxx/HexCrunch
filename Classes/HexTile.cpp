

#include "HexTileUtils.h"
#include "HexTileState.h"
#include "DrawingUtils.h"

USING_NS_CC;

namespace
{
	static constexpr auto Z_BG_HIGHLIGHT = 0;
	static constexpr auto Z_FOREGROUND = 1;
	static constexpr auto Z_CLIPPED = 2;
}



namespace hex
{
	bool HexTile::stateOnDeactivate()
	{
		switch (getCurrentState()) {
			case TileState::kNone:return HexTileState::initNoState(*this);;
			case TileState::kIdle: return HexTileState::turnOffIdle(*this);
			case TileState::kClipped: return HexTileState::turnOffClipped(*this);
			case TileState::kActing: return HexTileState::turnOffActing(*this);
			case TileState::kHighlighted: return HexTileState::turnOffHighlighted(*this);
			default: return false;
		}
	}


	bool HexTile::stateOnActivate(TileState pState)
	{
		if (pState == getCurrentState()) {
			return false;
		}
		switch (pState) {
			case TileState::kIdle: return HexTileState::turnOnIdle(*this);
			case TileState::kClipped: return HexTileState::turnOnClipped(*this);
			case TileState::kActing: return HexTileState::turnOnActing(*this);
			case TileState::kHighlighted: return HexTileState::turnOnHighlighted(*this);
			default: return false;
		}
	}
}



namespace hex
{
	HexTile::HexTile(const int pRingIndex, const int pTileIndex)
		: Hex(pRingIndex, pTileIndex)
		, m_clipped(nullptr)
		, m_background(nullptr)
		, m_foreground(nullptr)
		, m_bgHighlight(nullptr)
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

		m_bgHighlight = DrawNode::create();
		m_bgHighlight->setContentSize(sz);
		m_bgHighlight->setOpacity(255 * TILE_HIGHLIGHT_ALPHA);
		addChild(m_bgHighlight, Z_BG_HIGHLIGHT);

		m_foreground = Node::create();
		m_foreground->setContentSize(sz);
		addChild(m_foreground, Z_FOREGROUND);

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
		const auto tileStr = HexTileUtils::toStr(color);
		{
			auto tile = HexTileUtils::tileSprite(sz, tileStr);
			getForeground().removeAllChildren();
			getForeground().addChild(tile);
		}

		if (m_clipped != nullptr)
		{
			auto tile = HexTileUtils::tileSprite(sz, tileStr);
			getClipped().removeAllChildren();
			getClipped().addChild(tile);
		}

		if (m_pathLink.node != nullptr) {
			HexTileUtils::drawLinkCapsule(m_pathLink, color, { LINK_WIDTH, LINK_HEIGHT });
		}
	}


	void HexTile::setRingPathLink(const PathLink& pLink)
	{
		const auto& tileSz = getForeground().getContentSize();
		const auto clipSz = Size{ CLIP_WIDTH, CLIP_HEIGHT };
		const auto color = getColorId();
		const auto tileStr = HexTileUtils::toStr(color);

		m_clipped = Node::create();
		auto tile = HexTileUtils::tileSprite(tileSz, tileStr);
		getClipped().addChild(tile);

		auto capsule = DrawNode::create();
		auto clipped = HexTileUtils::createClipped(m_clipped, capsule);
		HexTileUtils::drawLinkCapsule({ pLink.angle, pLink.origin, capsule }, color, clipSz);
		addChild(clipped, Z_CLIPPED);

		const auto num = (getTileIndex() / (getRingIndex() + 1));
		const auto theta = (- 60.f * (1.f + float(num)));
		getClipped().setRotation(theta);
		getForeground().setRotation(theta);
		m_pathLink = pLink;
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