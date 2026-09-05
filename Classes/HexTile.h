#pragma once

#include "cocos2d.h"

#include "HexT.hpp"
#include "StateMachine.h"

namespace hex
{
	class HexTile : public Hex<HexTile>, public StateMachine<TileState, HexTile>, public cocos2d::Node
	{
		cocos2d::Node* m_blocked;
		cocos2d::Node* m_ringFace;
		cocos2d::Node* m_foreground;
		cocos2d::DrawNode* m_hexLink;
		cocos2d::DrawNode* m_background;

		void refreshView();
		void initClippedBg(cocos2d::Node* pGridNode);
		bool init(const ColorId pId, const int pRingIndex, const int pTileIndex);

	protected:

		HexTile(const ColorId pId, const int pRingIndex, const int pTileIndex);

		constexpr bool isBgEnabled() const;
		constexpr cocos2d::Node& getBlocked();
		constexpr cocos2d::Node& getBackground();
		constexpr cocos2d::DrawNode& getLink();

	public:

		void setState(TileState);
		void swapColors(HexTile&);
		void initRingPlacement(cocos2d::Node* pGridNode, cocos2d::Node* pLinkNode);

		static HexTile* create(const ColorId pId, const int pRingIndex, const int pTileIndex);

		constexpr cocos2d::Node& getRingFace();
		constexpr cocos2d::Node& getForeground();

		virtual bool stateOnDeactivate() = 0;
		virtual bool stateOnActivate(TileState) = 0;
	};
}