#pragma once

#include <array>

#include "Constants.h"

namespace hex
{
	template<class T>
	class Hex
	{
		const int m_ringIndex;
		const int m_tileIndex;
				
		T* m_next;
		T* m_previous;

		std::vector<T*> m_neighbours;

	protected:

		float m_edgeAngle;
		ColorId m_colorId;
		std::pair<float, float> m_linkPos;

	public:

		Hex(const ColorId, const int pRingIndex, const int pTileIndex);

		constexpr int getTileIndex() const;
		constexpr int getRingIndex() const;
		constexpr T* getNextRingTile() const;
		constexpr T* getPrevoiusRingTile() const;
		constexpr ColorId getColorId() const;
		constexpr float getHexRingEdgeAngle() const;

		void addNeighbour(T*);

		std::vector<T*> getInnerNeighbours();
		std::vector<T*> getOuterNeighbours();

		void setNextRingTile(T*);
	};
}