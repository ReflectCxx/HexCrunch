

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

	static ClippingNode* create_clipped(Node* pNode, Node* pStencil)
	{
		auto clippingNode = ClippingNode::create();
		clippingNode->setStencil(pStencil);
		clippingNode->setInverted(true);
		clippingNode->setAlphaThreshold(0.05f);
		clippingNode->addChild(pNode);
		return clippingNode;
	}

	static void show_index(hex::HexTile& pT)
	{
		const auto ri = pT.getRingIndex();
		const auto ti = pT.getTileIndex();
		std::string str;// = std::to_string(ri) + ", ";
		str += std::to_string(ti);
		auto label = Label::createWithTTF(str, hex::FONT, 50.f);
		//pT.addChild(label, 3);
	}
}


namespace hex
{
	HexTile::HexTile(const ColorId pId, const int pRingIndex, const int pTileIndex)
		: Hex(pId, pRingIndex, pTileIndex)
		, m_blocked(nullptr)
		, m_clipped(nullptr)
		, m_foreground(nullptr)
		, m_clippedBg(nullptr)
		, m_hexLink(nullptr)
		, m_ringFace(nullptr)
		, m_background(nullptr)
	{ }


	HexTile* HexTile::create(const ColorId pId, const int pRingIndex, const int pTileIndex)
	{
		auto pRet = new(std::nothrow) HexTState(pId, pRingIndex, pTileIndex);
		if (pRet && pRet->init(pId, pRingIndex, pTileIndex)) {
			pRet->autorelease();
		}
		else {
			delete pRet;
			pRet = nullptr;
		}
		return pRet;
	}
}



namespace hex
{
	void HexTile::swapColors(HexTile& pOther)
	{
		std::swap(m_colorId, pOther.m_colorId);
		pOther.refreshView();
		refreshView();
	}


	void HexTile::refreshView()
	{
		const auto& sz = getForeground().getContentSize();
		const auto color = getColorId();

		m_foreground->removeAllChildren();
		m_foreground->addChild(Asset::createNormalTile(color, sz));

		m_clipped->removeAllChildren();
		m_clipped->addChild(Asset::createNormalTile(color, sz));

		m_blocked->removeAllChildren();
		m_blocked->addChild(Asset::createBlockedTile(color, sz));

		auto& pt = *getPrevoiusRingTile();
		const auto linkSz = Size{ LINK_WIDTH, LINK_HEIGHT };
		const auto linkPos = Vec2{ pt.m_linkPos.first, pt.m_linkPos.second };
		Asset::drawHexLink(pt.m_hexLink, color, linkPos, linkSz, pt.m_edgeAngle);
	}


	bool HexTile::init(const ColorId pId, const int pRingIndex, const int pTileIndex)
	{
		if (!Node::init()) {
			return false;
		}

		constexpr auto radius = (HEX_RAD - HEX_BORDER);
		const auto sz = Size{ SQRT_3 * radius, 2.f * radius };
		
		m_foreground = Node::create();
		m_foreground->addChild(Asset::createNormalTile(pId, sz));
		m_foreground->setContentSize(sz);
		addChild(m_foreground, Z_FOREGROUND);
		
		m_clipped = Node::create();
		m_clipped->addChild(Asset::createNormalTile(pId, sz));

		m_ringFace = create_clipped(m_clipped, DrawNode::create());
		m_ringFace->setContentSize(sz);
		addChild(m_ringFace, Z_RING_ON);
		
		m_blocked = Node::create();
		m_blocked->setContentSize(sz);
		m_blocked->addChild(Asset::createBlockedTile(pId, sz));
		addChild(m_blocked, Z_BLOCKED);

		show_index(*this);
		return true;
	}


	void HexTile::initRingPlacement(Node* pGridNode, Node* pLinkNode)
	{
		const auto& nextTile = *getNextRingTile();
		const auto color = nextTile.getColorId();
		const auto pos = convertToNodeSpace(nextTile.convertToWorldSpace({ 0.f, 0.f }));
		m_edgeAngle = float(-atan(pos.y / pos.x) * (180.f / M_PI));

		const auto linkPos = Vec2{ pos.x / 2.f, pos.y / 2.f };
		m_linkPos = { linkPos.x, linkPos.y };

		m_hexLink = DrawNode::create();
		const auto linkSz = Size{ LINK_WIDTH, LINK_HEIGHT };
		Asset::drawHexLink(m_hexLink, color, linkPos, linkSz, m_edgeAngle);
		
		m_hexLink->setPosition(getPosition());
		pLinkNode->addChild(m_hexLink);

		const auto clipSz = Size{ LINK_CLIP_W, LINK_CLIP_H };
		const auto stencil = static_cast<DrawNode*>(m_ringFace->getStencil());
		Asset::drawHexLink(stencil, ColorId::None, linkPos, clipSz, m_edgeAngle);

		initClippedBg(pGridNode);

		const auto theta = getHexRingEdgeAngle();
		m_clipped->setRotation(theta);
		m_blocked->setRotation(theta);
		m_foreground->setRotation(theta);
	}


	void HexTile::initClippedBg(Node* pGridBgNode)
	{
		auto stencil = DrawNode::create();
		const auto clipSz = Size{ LINK_CLIP_W, LINK_CLIP_H };
		const auto pos = Vec2{ m_linkPos.first, m_linkPos.second };
		Asset::drawHexLink(stencil, ColorId::None, pos, clipSz, m_edgeAngle);

		const auto ti = getTileIndex();
		const auto ri = getRingIndex();
		if (ti % (ri + 1) == 0) {
			constexpr float theta = 120.f * (M_PI / 180.f);
			const auto rOrg = Vec2{
				pos.x * std::cos(theta) - pos.y * std::sin(theta),
				pos.x * std::sin(theta) + pos.y * std::cos(theta)
			};
			Asset::drawHexLink(stencil, ColorId::None, rOrg, clipSz, (m_edgeAngle + 60.f), false);
		}
		else {
			constexpr float theta = 180.f * (M_PI / 180.f);
			const auto rOrg = Vec2{
				pos.x * std::cos(theta) - pos.y * std::sin(theta),
				pos.x * std::sin(theta) + pos.y * std::cos(theta)
			};
			Asset::drawHexLink(stencil, ColorId::None, rOrg, clipSz, m_edgeAngle, false);
		}

		const auto sz = Size{ HEX_WIDTH, HEX_HEIGHT };
		m_clippedBg = Asset::createTileBg(sz);
		m_background = create_clipped(m_clippedBg, stencil);
		m_background->setPosition(getPosition());
		pGridBgNode->addChild(m_background);
	}
}