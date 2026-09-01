#pragma once

#include "cocos2d.h"

#include "Hex.h"

namespace hex
{
	struct PathLink {
		float angle = 0.f;
		cocos2d::Vec2 origin = {0.f, 0.f};
		cocos2d::DrawNode* node = nullptr;
	};


	class HexTile : public Hex, public cocos2d::Node
	{
		friend struct HexTileUtils;
	protected:

		PathLink m_pathLink;

		cocos2d::Node* m_clipped;
		cocos2d::Node* m_highlight;
		cocos2d::Node* m_background;
		cocos2d::Node* m_foreground;

		void addIndexLabel();
		void refreshTileColor();

		HexTile(const int pRingIndex, const int pTileIndex);

		bool init(const ColorId pId, const int pRingIndex, const int pTileIndex);

	public:

		SETP(cocos2d::Node, Background, m_background)

		GETPREF(cocos2d::Node, Clipped, m_clipped)
		GETPREF(cocos2d::Node, Link, m_pathLink.node)
		GETPREF(cocos2d::Node, Highlight, m_highlight)
		GETPREF(cocos2d::Node, Background, m_background)
		GETPREF(cocos2d::Node, Foreground, m_foreground)

		void setState(TileState pState);

		void setRingPathLink(const PathLink& pLink);

		static HexTile* create(const ColorId pId, const int pRingIndex, const int pTileIndex);
	};
}