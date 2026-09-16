

#include "Asset.h"
#include "HexTile.h"
#include "HexTState.h"
#include "DrawingUtils.h"

USING_NS_CC;

namespace
{
	static constexpr auto Z_FOREGROUND = 0;
	static constexpr auto Z_RING_ON = 1;
	static constexpr auto Z_BLOCKED = 2;
	static constexpr auto DBG_LABEL_TAG = 999;

	static ClippingNode* create_clipped(Node* pNode, Node* pStencil)
	{
		auto clippingNode = ClippingNode::create();
		clippingNode->setStencil(pStencil);
		clippingNode->setInverted(true);
		clippingNode->setAlphaThreshold(0.05f);
		clippingNode->addChild(pNode);
		return clippingNode;
	}
}


namespace hex
{
	HexTile::HexTile(const int pRingIndex, const int pTileIndex)
		: Hex(pRingIndex, pTileIndex)
		, m_meta{ ColorId::None, ColorId::None, 0.f, {0.f, 0.f} }
		, m_arrow(nullptr)
		, m_hexIdle(nullptr)
		, m_hexBlocked(nullptr)
		, m_hexClipped(nullptr)
		, m_hexLink(nullptr)
		, m_ringFace(nullptr)
		, m_background(nullptr)
	{ }


	HexTile* HexTile::create(const int pRingIndex, const int pTileIndex)
	{
		auto pRet = new(std::nothrow) HexTState(pRingIndex, pTileIndex);
		if (pRet && pRet->init(pRingIndex, pTileIndex)) {
			pRet->autorelease();
		}
		else {
			delete pRet;
			pRet = nullptr;
		}
		return pRet;
	}


	void HexTile::showString(const std::string& pStr)
	{
		auto label = getChildByTag(DBG_LABEL_TAG);
		if (label == nullptr) {
			label = Label::createWithTTF(pStr, hex::FONT, 50.f);
			addChild(label, Z_BLOCKED + 1);
		}
		else {
			static_cast<Label*>(label)->setString(pStr);
		}
	}
}



namespace hex
{
	void HexTile::swapColor(HexTile& pOther, bool pRefreshView)
	{
		std::swap(m_meta.color, pOther.m_meta.color);
		if (pRefreshView) {
			pOther.refreshView();
			refreshView();
		}
	}


	void HexTile::refreshView()
	{
		const auto color = getColorId();
		if (color == ColorId::None) {
			return;
		}
		const auto& sz = getIdleFace().getContentSize();
		
		m_hexIdle->removeAllChildren();
		m_hexIdle->addChild(Asset::createNormalTile(color, sz));

		m_hexClipped->removeAllChildren();
		m_hexClipped->addChild(Asset::createNormalTile(color, sz));

		m_hexBlocked->removeAllChildren();
		m_hexBlocked->addChild(Asset::createBlockedTile(color, sz));

		auto& pt = *getPrevoiusRingTile();
		const auto linkSz = Size{ LINK_WIDTH, LINK_HEIGHT };
		const auto linkPos = Vec2{ pt.m_meta.linkPos.first, pt.m_meta.linkPos.second };
		Asset::drawHexLink(pt.m_hexLink, color, linkPos, linkSz, pt.m_meta.edgeAngle);
	}


	bool HexTile::init(const int pRingIndex, const int pTileIndex)
	{
		if (!Node::init()) {
			return false;
		}

		constexpr auto radius = float(HEX_RAD - HEX_BORDER);
		const auto sz = Size{ SQRT_3 * radius, 2.f * radius };
		
		m_hexIdle = Node::create();
		m_hexIdle->setContentSize(sz);
		addChild(m_hexIdle, Z_FOREGROUND);
		
		m_hexClipped = Node::create();
		m_ringFace = create_clipped(m_hexClipped, DrawNode::create());
		m_ringFace->setContentSize(sz);
		addChild(m_ringFace, Z_RING_ON);
		
		m_hexBlocked = Node::create();
		m_hexBlocked->setContentSize(sz);
		addChild(m_hexBlocked, Z_BLOCKED);

		m_arrow = Asset::createArrow();
		m_arrow->setRotation(getArrowAngle());
		addChild(m_arrow, Z_BLOCKED + 1);
		m_arrow->setVisible(false);
		return true;
	}


	void HexTile::initRingPlacement(Node* pGridNode, Node* pLinkNode)
	{
		const auto& nextTile = *getNextRingTile();
		const auto pos = convertToNodeSpace(nextTile.convertToWorldSpace({ 0.f, 0.f }));
		m_meta.edgeAngle = float(-atan(pos.y / pos.x) * (180.f / M_PI));

		const auto linkPos = Vec2{ pos.x / 2.f, pos.y / 2.f };
		m_meta.linkPos = { linkPos.x, linkPos.y };

		m_hexLink = DrawNode::create();
		const auto linkSz = Size{ LINK_WIDTH, LINK_HEIGHT };
		Asset::drawHexLink(m_hexLink, ColorId::None, linkPos, linkSz, m_meta.edgeAngle);
		
		m_hexLink->setPosition(getPosition());
		pLinkNode->addChild(m_hexLink, getLocalZOrder());

		const auto clipSz = Size{ LINK_CLIP_W, LINK_CLIP_H };
		const auto stencil = static_cast<DrawNode*>(m_ringFace->getStencil());
		Asset::drawHexLink(stencil, ColorId::None, linkPos, clipSz, m_meta.edgeAngle);

		initClippedBg(pGridNode);

		const auto theta = getHexRingEdgeAngle();
		m_hexClipped->setRotation(theta);
		m_hexBlocked->setRotation(theta);
		m_hexIdle->setRotation(theta);
	}


	void HexTile::initClippedBg(Node* pGridBgNode)
	{
		auto stencil = DrawNode::create();
		const auto clipSz = Size{ LINK_CLIP_W, LINK_CLIP_H };
		const auto pos = Vec2{ m_meta.linkPos.first, m_meta.linkPos.second };
		Asset::drawHexLink(stencil, ColorId::None, pos, clipSz, m_meta.edgeAngle);

		const auto ti = getTileIndex();
		const auto ri = getRingIndex();
		if (ti % (ri + 1) == 0) {
			constexpr float theta = 120.f * float(M_PI / 180.f);
			const auto rOrg = Vec2{
				pos.x * std::cos(theta) - pos.y * std::sin(theta),
				pos.x * std::sin(theta) + pos.y * std::cos(theta)
			};
			Asset::drawHexLink(stencil, ColorId::None, rOrg, clipSz, (m_meta.edgeAngle + 60.f), false);
		}
		else {
			constexpr float theta = 180.f * float(M_PI / 180.f);
			const auto rOrg = Vec2{
				pos.x * std::cos(theta) - pos.y * std::sin(theta),
				pos.x * std::sin(theta) + pos.y * std::cos(theta)
			};
			Asset::drawHexLink(stencil, ColorId::None, rOrg, clipSz, m_meta.edgeAngle, false);
		}

		const auto sz = Size{ HEX_WIDTH, HEX_HEIGHT };
		const auto clippedBg = Asset::createTileBg(sz);
		m_background = create_clipped(clippedBg, stencil);
		m_background->setPosition(getPosition());
		pGridBgNode->addChild(m_background, getLocalZOrder());
	}
}