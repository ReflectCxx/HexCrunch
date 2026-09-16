#pragma once

#include "cocos2d.h"

#include "HexT.hpp"

namespace hex
{
	class HexTile : public Hex<HexTile>, public cocos2d::Node
	{
		struct HexMeta
		{
			ColorId color;
			ColorId spawnColor;

			float edgeAngle;
			std::pair<float, float> linkPos;
		};

		HexMeta m_meta;
		cocos2d::Node* m_arrow;
		cocos2d::Node* m_hexIdle;
		cocos2d::Node* m_hexBlocked;
		cocos2d::Node* m_hexClipped;
		cocos2d::DrawNode* m_hexLink;
		cocos2d::ClippingNode* m_ringFace;
		cocos2d::ClippingNode* m_background;

		void initClippedBg(cocos2d::Node* pGridNode);
		bool init(const int pRingIndex, const int pTileIndex);

	protected:

		HexTile(const int pRingIndex, const int pTileIndex);

		constexpr cocos2d::Node& getBlockedFace();

	public:

		void refreshView();
		void swapColor(HexTile&, bool pRefreshView);
		void initRingPlacement(cocos2d::Node* pGridNode, cocos2d::Node* pLinkNode);

		static HexTile* create(const int pRingIndex, const int pTileIndex);

		const float getArrowAngle() const;
		constexpr cocos2d::Node& getArrow();
		constexpr cocos2d::Node& getRingFace();
		constexpr cocos2d::Node& getIdleFace();
		constexpr cocos2d::Node& getBackground();
		constexpr cocos2d::Node& getClippedFace();
		constexpr cocos2d::DrawNode& getLink();

		constexpr ColorId getColorId() const;
		constexpr ColorId getSpawnColor() const;

		constexpr void setColorId(ColorId);
		constexpr void setSpawnColor(ColorId);
		constexpr float getHexRingEdgeAngle() const;
		constexpr void assignColor(const ColorId pColor);

		virtual void setState(const TileState) = 0;
		virtual const TileState getState() = 0;

		void showString(const std::string& pStr);
	};
}