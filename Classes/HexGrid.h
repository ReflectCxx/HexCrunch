#pragma once

#include "SwipeTracker.h"
#include "SwipeConsumer.h"
#include "HexGridController.h"

namespace hex
{
	class HexGrid : public cocos2d::Node
	{
		HexgonRingMatrix m_hexRings;

		HexGridController m_controller;

		std::unique_ptr<SwipeTracker> m_swipeTracker = nullptr;

		std::unique_ptr<SwipeConsumer> m_swipeConsumer = nullptr;

		HexGrid();

		bool init() override;

		void initHexGrid();

		HexTile* spawnNewTile(const cocos2d::Vec2& pos, 
							  const int pRingIndex, const int pTileIndex);

		void initClippedBGTile(cocos2d::Node* pBgNode, HexTile* pTile,
							   const cocos2d::Vec2& pOrigin, int pRingIndex, float pAngle);

		void initHexTileBackground(cocos2d::Node* pBgNode, 
								   cocos2d::Node* pLinkNode, int pRingIndex);

		void linkNeighbouringRingTiles(const NeighboursMat& pFaceCounts);

	public:

		GET(HexgonRingMatrix, HexagonRings, m_hexRings);

		CREATE_FUNC(HexGrid)

		constexpr HexGridController& controller();
	};
}




namespace hex
{
	constexpr HexGridController& HexGrid::controller() {
		return m_controller;
	}
}