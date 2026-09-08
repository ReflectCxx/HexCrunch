#pragma once

#include "cocos2d.h"
#include "Constants.h"
#include "GridManager.h"

namespace hex
{
	class HexGrid : public cocos2d::Node
	{
		GridManager m_manager;

		HexRingMatrix m_hexRings;

		bool init() override;

		void initHexGrid();

		HexTile* spawnNewTile(const cocos2d::Vec2& pPos,
							  const int pRingIndex, const int pTileIndex);

		void initRingHexTiles(cocos2d::Node* pBgNode,
						      cocos2d::Node* pLinkNode, int pRingIndex);

		void linkNeighbouringRingTiles(const NeighboursMat& pFaceCounts);

	public:

		constexpr GridManager& manager();
		constexpr const HexRingMatrix& getHexagonRings() const;
		
		CREATE_FUNC(HexGrid)

		void update(float) override;
	};
}


namespace hex
{
	constexpr const HexRingMatrix& HexGrid::getHexagonRings() const {
		return m_hexRings;
	};

	constexpr GridManager& HexGrid::manager() {
		return m_manager;
	}
}