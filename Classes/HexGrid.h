#pragma once

#include "cocos2d.h"
#include "Constants.h"

namespace hex
{
	class HexGrid : public cocos2d::Node
	{
		HexRingMatrix m_hexRings;

		bool init() override;

		void initHexGrid();

		HexTile* spawnNewTile(const cocos2d::Vec2& pos,
							  const int pRingIndex, const int pTileIndex);

		void initRingHexTiles(cocos2d::Node* pBgNode,
						      cocos2d::Node* pLinkNode, int pRingIndex);

		void linkNeighbouringRingTiles(const NeighboursMat& pFaceCounts);

	public:

		constexpr const HexRingMatrix& getHexagonRings() const;

		CREATE_FUNC(HexGrid)
	};
}



namespace hex
{
	inline constexpr const HexRingMatrix& HexGrid::getHexagonRings() const {
		return m_hexRings;
	};
}