

#include "Game.h"
#include "HexGrid.h"
#include "HexTile.h"
#include "DrawingUtils.h"

USING_NS_CC;

namespace
{
	constexpr auto BG_NODE_Z = 0;
	constexpr auto LINK_NODE_Z = 1;
	constexpr auto HEXGRID_NODE_Z = 2;
}


namespace hex
{
	void HexGrid::update(float)
	{
		m_manager.update();
	}

	bool HexGrid::init()
	{
		if (!Node::init()) {
			return false;
		}

		initHexGrid();

		Game::instance().loadLevel(m_hexRings);

		auto& actor = *m_hexRings[RING_COUNT - 2][0];
		auto& follower = *m_hexRings[RING_COUNT - 1][0];
		actor.swapColor(follower, false);

		for (auto& rings : m_hexRings) {
			for (const auto t : rings) {
				t->setState(TileState::Idle);
			}
		}

		setRotation(30.f);
		scheduleUpdate();
		return true;
	}
}


namespace hex
{
	void HexGrid::initRingHexTiles(int pRingIndex)
	{
		auto startTile = m_hexRings[pRingIndex][0];
		auto currentTile = startTile;
		do {
			currentTile->initRingPlacement(m_hexBGNode, m_hexLinkNode);
			currentTile = currentTile->getNextRingTile();;
		} while (currentTile != startTile);
	}


	HexTile* HexGrid::spawnNewTile(const int pRingIndex, const int pTileIndex, const cocos2d::Vec2& pPos)
	{
		constexpr auto HEX_TILES_COUNT = (HEX_6 / 2) * (RING_COUNT + 1) * (RING_COUNT + 2);
		const auto count = ((HEX_6 / 2) * pRingIndex * (pRingIndex + 1) + pTileIndex + 1);
		const auto zOrder = (HEX_TILES_COUNT - count);

		auto tile = HexTile::create(pRingIndex, pTileIndex);
		tile->setPosition(pPos);
		tile->setLocalZOrder(zOrder);
		m_hexNode->addChild(tile);
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
				for (int faceCount = 0; faceCount < pFaceCounts[ringIndex][index]; faceCount++) {
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
		m_hexBGNode = Node::create();
		addChild(m_hexBGNode, BG_NODE_Z);

		m_hexLinkNode = Node::create();
		addChild(m_hexLinkNode, LINK_NODE_Z);

		m_hexNode = Node::create();
		addChild(m_hexNode, HEXGRID_NODE_Z);

		std::vector<std::vector<int>> outwardNeighboursMatrix;
		for (int ringIndex = 0; ringIndex <= RING_COUNT; ringIndex++)
		{
			int tileIndex = 0;
			HexTile* previousTile = nullptr;

			std::vector<HexTile*> ringTiles;
			std::vector<int> tileOutwardNeighbourCount;

			for (int angleI = 0; angleI < HEX_6; angleI++)
			{
				const auto theta = float(angleI * (M_PI / 3.f));
				const float centerPosX = HEX_WIDTH * (ringIndex + 1) * cos(theta);
				const float centerPosY = HEX_WIDTH * (ringIndex + 1) * sin(theta);

				auto nextTile = spawnNewTile(ringIndex, tileIndex, { centerPosX, centerPosY });
				ringTiles.push_back(nextTile);
				tileOutwardNeighbourCount.push_back(3);
				tileIndex++;

				if (previousTile) {
					previousTile->setNextRingTile(nextTile);
				}
				previousTile = nextTile;

				for (int i = 1; i <= ringIndex; i++) {
					const auto beta = float(theta + (M_PI * 2.f/ 3.f));
					const float adjPosX = centerPosX + i * HEX_WIDTH * cos(beta);
					const float adjPosY = centerPosY + i * HEX_WIDTH * sin(beta);
					auto nextTile = spawnNewTile(ringIndex, tileIndex, { adjPosX, adjPosY });

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
			initRingHexTiles(ringIndex);
		}
		linkNeighbouringRingTiles(outwardNeighboursMatrix);
	}
}