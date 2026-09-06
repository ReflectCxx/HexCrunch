
#include <random>
#include <utility>

#include "HexGrid.h"
#include "HexTile.h"
#include "DrawingUtils.h"

USING_NS_CC;


namespace
{
	static std::vector<hex::ColorId>& get_colors() 
	{
		static auto colors = []()->auto {

			std::vector<int> count = { 6, 12, 18, 24, 30 };		//total tiles 90 tiles.
			std::vector<hex::ColorId> arr = {
				hex::ColorId::Red,
				hex::ColorId::Green,
				hex::ColorId::Yellow,
				hex::ColorId::Blue,
				hex::ColorId::Purple
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
		}();

		return colors;
	}
}


namespace hex
{
	bool HexGrid::init()
	{
		if (!Node::init()) {
			return false;
		}
		initHexGrid();
		setRotation(30.f);
		return true;
	}
}


namespace hex
{
	void HexGrid::initRingHexTiles(Node* pBgNode, cocos2d::Node* pLinkNode, int pRingIndex)
	{
		auto startTile = m_hexRings[pRingIndex][0];
		auto currentTile = startTile;
		do {
			currentTile->initRingPlacement(pBgNode, pLinkNode);
			currentTile->setState(TileState::Idle);
			currentTile = currentTile->getNextRingTile();;
		} while (currentTile != startTile);
	}


	HexTile* HexGrid::spawnNewTile(const cocos2d::Vec2& pos, const int pRingIndex, const int pTileIndex)
	{
		auto& colors = get_colors();
		auto colorId = ColorId::None;
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
					previousTile->setNextRingTile(nextTile);
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
						previousTile->setNextRingTile(nextTile);
					}
					previousTile = nextTile;
				}
			}

			m_hexRings.push_back(ringTiles);
			outwardNeighboursMatrix.push_back(tileOutwardNeighbourCount);
			previousTile->setNextRingTile(ringTiles.front());
			initRingHexTiles(hexGridBGNode, hexGridLinkNode, ringIndex);
		}
		linkNeighbouringRingTiles(outwardNeighboursMatrix);
	}
}