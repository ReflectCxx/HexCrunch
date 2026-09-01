
#include "HexGrid.h"
#include "HexTileUtils.h"
#include "DrawingUtils.h"

USING_NS_CC;

namespace {
	static constexpr auto CORNER_RAD = 15.f;
}

namespace
{
	std::vector<hex::ColorId> getRandomColors()
	{
		std::vector<int> count = { 6, 12, 18, 24, 30 };
		std::vector<hex::ColorId> arr = {
			hex::ColorId::kRed,
			hex::ColorId::kGreen,
			hex::ColorId::kYellow,
			hex::ColorId::kBlue,
			hex::ColorId::kPurple
		};

		std::random_device rd;
		std::mt19937 rng(rd());
		std::shuffle(count.begin(), count.end(), rng);

		std::vector<hex::ColorId> colorBag;
		for (size_t i = 0; i < arr.size(); ++i) {
			colorBag.insert(colorBag.end(), count[i], arr[i]);
		}
		std::shuffle(colorBag.begin(), colorBag.end(), rng);
		return colorBag;
	}
}


namespace hex
{
	HexGrid::HexGrid()
		: m_controller(*this)
		, m_swipeConsumer(std::make_unique<SwipeConsumer>(*this))
		, m_swipeTracker(std::make_unique<SwipeTracker>(this,
			[this](Swipe pDir) {
				m_swipeConsumer->onInputRecieved(pDir);
			}))
	{ }

	bool HexGrid::init()
	{
		if (!Node::init()) {
			return false;
		}
		initHexGrid();
		setRotation(30.f);
		m_swipeConsumer->init();
		return true;
	}
}


namespace hex
{
	HexTile* HexGrid::spawnNewTile(const cocos2d::Vec2& pos, const int pRingIndex, const int pTileIndex)
	{
		static auto colors = getRandomColors();
		auto colorId = ColorId::kNone;
		if (pRingIndex < RING_COUNT) {
			colorId = colors.back();
			colors.pop_back();
		}

		auto tile = HexTile::create(colorId, pRingIndex, pTileIndex);
		tile->setPosition(pos);
		addChild(tile);
		if (pRingIndex == RING_COUNT) {
			tile->setVisible(false);
		}
		return tile;
	}


	void HexGrid::linkNeighbouringRingTiles(const NeighboursMat& pFaceCounts)
	{
		for (size_t ringIndex = 0; ringIndex < RING_COUNT; ringIndex++)
		{
			const std::vector<HexTile*>& innerRing = m_hexRings[ringIndex];
			const std::vector<HexTile*>& outerRing = m_hexRings[ringIndex + 1];
			int indexCounter = outerRing.size() - 1;

			for (size_t index = 0; index < innerRing.size(); index++)
			{
				HexTile* innerTile = innerRing[index];
				for (size_t faceCount = 0; faceCount < pFaceCounts[ringIndex][index]; faceCount++) {
					const int outerRingIndex = indexCounter % outerRing.size();
					innerTile->addNeighbour(outerRing[outerRingIndex]);
					outerRing[outerRingIndex]->addNeighbour(innerTile);
					indexCounter++;
				}
				indexCounter--;
			}
		}
	}


	void HexGrid::initHexTileBackground(Node* pBgNode, cocos2d::Node* pLinkNode, int pRingIndex)
	{
		auto startTile = m_hexRings[pRingIndex][0];
		auto currentTile = startTile;
		do
		{
			const auto nextTile = static_cast<HexTile*>(currentTile->getNextRingTile());
			const auto ntWpos = nextTile->convertToWorldSpace(cocos2d::Vec2::ZERO);
			const auto ntNpos = currentTile->convertToNodeSpace(ntWpos);
			const auto angle = static_cast<float>(-atan(ntNpos.y / ntNpos.x) * (180.f / M_PI));
			const auto origin = Vec2{ ntNpos.x / 2.f, ntNpos.y / 2.f };

			initClippedBGTile(pBgNode, currentTile,  origin, pRingIndex, angle);

			const auto node = DrawNode::create();
			const auto color = currentTile->getColorId();
			const auto pathLink = PathLink{ angle, origin, node };
			currentTile->setRingPathLink(pathLink);

			HexTileUtils::drawLinkCapsule(pathLink, color, { LINK_WIDTH, LINK_HEIGHT });
			node->setPosition(currentTile->getPosition());
			pLinkNode->addChild(node);

			currentTile->setState(TileState::kIdle);
			currentTile = nextTile;

		} while (currentTile != startTile);
	}


	void HexGrid::initClippedBGTile(Node* pBgNode, HexTile* pTile, 
									const cocos2d::Vec2& pOrigin, int pRingIndex, float pAngle)
	{
		const auto node = DrawNode::create();
		ut::draw_hexagon(node, HEX_RAD, Color4F::WHITE, CORNER_RAD);
		node->setOpacity(255 * 0.9f);

		auto capsule = DrawNode::create();
		auto clipped = HexTileUtils::createClipped(node, capsule);
		HexTileUtils::drawLinkCapsule({ pAngle, pOrigin, capsule }, ColorId::kNone, { CLIP_WIDTH, CLIP_HEIGHT });

		const auto ti = pTile->getTileIndex();
		if (ti % (pRingIndex + 1) == 0)
		{
			constexpr float theta = 120.f * (M_PI / 180.f);
			const auto rOrg = Vec2{
				pOrigin.x * std::cos(theta) - pOrigin.y * std::sin(theta),
				pOrigin.x * std::sin(theta) + pOrigin.y * std::cos(theta)
			};
			HexTileUtils::drawLinkCapsule({ pAngle + 60.f, rOrg, capsule }, ColorId::kNone, { CLIP_WIDTH, CLIP_HEIGHT }, false);
		}
		else
		{
			constexpr float theta = 180.f * (M_PI / 180.f);
			const auto rOrg = Vec2{
				pOrigin.x * std::cos(theta) - pOrigin.y * std::sin(theta),
				pOrigin.x * std::sin(theta) + pOrigin.y * std::cos(theta)
			};
			HexTileUtils::drawLinkCapsule({ pAngle, rOrg, capsule }, ColorId::kNone, { CLIP_WIDTH, CLIP_HEIGHT }, false);
		}

		clipped->setPosition(pTile->getPosition());
		pBgNode->addChild(clipped);
		pTile->setBackground(node);
	}


	void HexGrid::initHexGrid()
	{
		std::vector<std::vector<int>> outwardNeighboursMatrix;

		auto hexGridBGNode = Node::create();
		addChild(hexGridBGNode);

		auto hexGridLinkNode = Node::create();
		addChild(hexGridLinkNode);

		for (int ringIndex = 0; ringIndex <= RING_COUNT; ringIndex++)
		{
			int tileIndex = 0;
			HexTile* previousTile = nullptr;

			std::vector<HexTile*> ringTiles;
			std::vector<int> tileOutwardNeighbourCount;

			for (int angleI = 0; angleI < HEX_6; angleI++)
			{
				const float theta = angleI * (M_PI / 3.f);
				const float centerPosX = HEX_WIDTH * (ringIndex + 1) * cos(theta);
				const float centerPosY = HEX_WIDTH * (ringIndex + 1) * sin(theta);

				auto nextTile = spawnNewTile({ centerPosX, centerPosY }, ringIndex, tileIndex);
				ringTiles.push_back(nextTile);
				tileOutwardNeighbourCount.push_back(3);
				tileIndex++;

				if (previousTile) {
					Hex::initRingPath(previousTile, nextTile);
				}
				previousTile = nextTile;

				for (int i = 1; i <= ringIndex; i++) {
					const float beta = theta + (M_PI * 2.f/ 3.f);
					const float adjPosX = centerPosX + i * HEX_WIDTH * cos(beta);
					const float adjPosY = centerPosY + i * HEX_WIDTH * sin(beta);
					auto nextTile = spawnNewTile({ adjPosX, adjPosY }, ringIndex, tileIndex);

					ringTiles.push_back(nextTile);
					tileOutwardNeighbourCount.push_back(2);
					tileIndex++;

					if (previousTile) {
						Hex::initRingPath(previousTile, nextTile);
					}
					previousTile = nextTile;
				}
			}

			m_hexRings.push_back(ringTiles);
			outwardNeighboursMatrix.push_back(tileOutwardNeighbourCount);
			Hex::initRingPath(previousTile, ringTiles.front());
			if (ringIndex != RING_COUNT) {
				initHexTileBackground(hexGridBGNode, hexGridLinkNode, ringIndex);
			}
		}
		linkNeighbouringRingTiles(outwardNeighboursMatrix);
	}
}