#pragma once

#include <unordered_set>

#include "Constants.h"

namespace hex
{
	class Hex
	{
		const int m_ringIndex;
		const int m_tileIndex;

		Hex* m_next;
		Hex* m_previous;

		std::unordered_set<Hex*> m_neighbours;

	protected:

		ColorId m_tileId = ColorId::kNone;

		SET(ColorId, ColorId, m_tileId);
		
	public:

		Hex(const int pRingIndex, const int pTileIndex);

		GET(ColorId, ColorId, m_tileId);
		GET(int, RingIndex, m_ringIndex);
		GET(int, TileIndex, m_tileIndex);

		GETP(Hex, NextRingTile, m_next);
		GETP(Hex, PrevoiusRingTile, m_previous);

		GET(std::unordered_set<Hex*>, Neighbours, m_neighbours);

		void addNeighbour(Hex*);

		std::vector<Hex*> getInnerNeighbours();
		std::vector<Hex*> getOuterNeighbours();

		static void initRingPath(Hex* pPrevious, Hex* Next);
	};
}