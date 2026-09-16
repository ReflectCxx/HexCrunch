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

		cocos2d::Node* m_hexNode = nullptr;
		cocos2d::Node* m_hexBGNode = nullptr;
		cocos2d::Node* m_hexLinkNode = nullptr;

		bool init() override;

		void initHexGrid();

		HexTile* spawnNewTile(const int pRingIndex, const int pTileIndex,
							  const cocos2d::Vec2& pPos);

		void initRingHexTiles(int pRingIndex);

		void linkNeighbouringRingTiles(const NeighboursMat& pFaceCounts);

	public:
		
		constexpr GridManager& manager();
		constexpr const HexRingMatrix& getHexagonRings() const;
		
		constexpr cocos2d::Node& getHexNode();
		constexpr cocos2d::Node& getHexBGNode();

		CREATE_FUNC(HexGrid)

		void update(float) override;
	};
}


namespace hex
{
	constexpr const HexRingMatrix& HexGrid::getHexagonRings() const {
		return m_hexRings;
	}

	constexpr cocos2d::Node& HexGrid::getHexNode() {
		return *m_hexNode;
	}

	constexpr cocos2d::Node& HexGrid::getHexBGNode() {
		return *m_hexBGNode;
	}

	constexpr GridManager& HexGrid::manager() {
		return m_manager;
	}
}