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

	public:

		Hex(const int pRingIndex, const int pTileIndex);

		constexpr int getTileIndex() const;
		constexpr int getRingIndex() const;
		
		constexpr T* getNextRingTile() const;
		constexpr T* getPrevoiusRingTile() const;

		void addNeighbour(T*);
		
		std::vector<T*> getInnerNeighbours();
		std::vector<T*> getOuterNeighbours();
		constexpr const std::vector<T*>& getNeighbours() const;

		void setNextRingTile(T*);
	};
}