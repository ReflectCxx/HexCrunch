
#include <algorithm>

#include "Hex.h"

namespace hex
{
	Hex::Hex(const int pRingIndex, const int pTileIndex)
		: m_ringIndex(pRingIndex)
		, m_tileIndex(pTileIndex)
		, m_next(nullptr)
		, m_previous(nullptr)
	{ }


	void Hex::addNeighbour(Hex* pTile)
	{
		m_neighbours.insert(pTile);
	}


	void Hex::initRingPath(Hex* pPrevious, Hex* pNext)
	{
		pPrevious->m_next = pNext;
		pNext->m_previous = pPrevious;

		pNext->addNeighbour(pPrevious);
		pPrevious->addNeighbour(pNext);
	}


	std::vector<Hex*> Hex::getInnerNeighbours()
	{
		auto arr = std::vector<Hex*>();
		for (auto tile : m_neighbours) {
			if (tile->getRingIndex() < getRingIndex()) {
				arr.push_back(tile);
			}
		}
		if (arr.size() > 1) 
		{
			if (abs(arr[0]->getTileIndex() - arr[1]->getTileIndex()) > 1) {
				if (arr[0]->getTileIndex() < arr[1]->getTileIndex()) {
					std::swap(arr[0], arr[1]);
				}
			}
			else if (arr[0]->getTileIndex() > arr[1]->getTileIndex()) {
				std::swap(arr[0], arr[1]);
			}
		}
		return arr;
	}


	std::vector<Hex*> Hex::getOuterNeighbours()
	{
		auto arr = std::vector<Hex*>();
		for (auto tile : m_neighbours) {
			if (tile->getRingIndex() > getRingIndex()) {
				arr.push_back(tile);
			}
		}
		const auto cmp = [](const Hex* a, const Hex* b) {
			return (a->getTileIndex() < b->getTileIndex());
		};
		std::sort(arr.begin(), arr.end(), cmp);
		if (getTileIndex() == 0) {
			auto tile = arr.back();
			arr.pop_back();
			arr.insert(arr.begin(), tile);
		}
		return arr;
	}
}