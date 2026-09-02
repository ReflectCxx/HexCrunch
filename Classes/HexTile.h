#pragma once

#include "cocos2d.h"

#include "Hex.h"
#include "StateMachine.h"

namespace hex
{
	struct PathLink {
		float angle = 0.f;
		cocos2d::Vec2 origin = {0.f, 0.f};
		cocos2d::DrawNode* node = nullptr;
	};


	class HexTile : public Hex, public StateMachine<TileState, HexTile>, public cocos2d::Node
	{
		friend struct HexTileUtils;
		friend struct HexTileState;

	protected:

		PathLink m_pathLink;

		cocos2d::Node* m_clipped;
		cocos2d::Node* m_foreground;
		cocos2d::Node* m_background;
		cocos2d::DrawNode* m_bgHighlight;

		void addIndexLabel();
		void refreshTileColor();

		HexTile(const int pRingIndex, const int pTileIndex);

		bool init(const ColorId pId, const int pRingIndex, const int pTileIndex);

	public:

		SETP(cocos2d::DrawNode, Background, m_background)

		GETPREF(cocos2d::Node, Clipped, m_clipped)
		GETPREF(cocos2d::Node, Foreground, m_foreground)
		GETPREF(cocos2d::Node, Background, m_background)
		GETPREF(cocos2d::DrawNode, Highlight, m_bgHighlight)
		GETPREF(cocos2d::DrawNode, Link, (m_pathLink.node))

		void setState(TileState pState) {
			switchToState(pState);
		}

		bool stateOnDeactivate();

		bool stateOnActivate(TileState pState);

		void setRingPathLink(const PathLink& pLink);

		static HexTile* create(const ColorId pId, const int pRingIndex, const int pTileIndex);
	};
}