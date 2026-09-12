
#pragma once

#include <algorithm>

#include "HexT.h"

namespace hex
{
	template<class T>
	Hex<T>::Hex(const ColorId pColorId, const int pRingIndex, const int pTileIndex)
		: m_ringIndex(pRingIndex)
		, m_tileIndex(pTileIndex)
		, m_next(nullptr)
		, m_previous(nullptr)
		, m_edgeAngle(0.f)
		, m_linkPos{ 0.f, 0.f }
		, m_colorId(pColorId)
	{
		m_neighbours.reserve(HEX_6);
	}


	template<class T>
	constexpr int Hex<T>::getRingIndex() const {
		return m_ringIndex;
	};


	template<class T>
	constexpr int Hex<T>::getTileIndex() const {
		return m_tileIndex;
	};


	template<class T>
	constexpr T* Hex<T>::getNextRingTile() const {
		return m_next;
	};


	template<class T>
	constexpr T* Hex<T>::getPrevoiusRingTile() const {
		return m_previous;
	};


	template<class T>
	constexpr ColorId Hex<T>::getColorId() const {
		return m_colorId;
	}


	template<class T>
	constexpr const std::vector<T*>& Hex<T>::getNeighbours() const {
		return m_neighbours;
	}


	template<class T>
	inline void Hex<T>::addNeighbour(T* pTile) {
		if (std::find(m_neighbours.begin(), m_neighbours.end(), pTile) == m_neighbours.end()) {
			m_neighbours.push_back(pTile);
		}
	}


	template<class T>
	inline constexpr float Hex<T>::getHexRingEdgeAngle() const
	{
		const auto num = (m_tileIndex / (m_ringIndex + 1));
		const auto theta = (-60.f * (1.f + float(num)));
		return theta;
	}


	template<class T>
	inline void Hex<T>::setNextRingTile(T* pNext)
	{
		m_next = pNext;
		pNext->m_previous = static_cast<T*>(this);
		pNext->addNeighbour(static_cast<T*>(this));
		addNeighbour(pNext);
	}


	template<class T>
	inline std::vector<T*> Hex<T>::getOuterNeighbours()
	{
		auto arr = std::vector<T*>();
		for (auto tile : m_neighbours) {
			if (tile->getRingIndex() > getRingIndex()) {
				arr.push_back(tile);
			}
		}

		std::sort(arr.begin(), arr.end(), [](const T* a, const T* b){
			return (a->getTileIndex() < b->getTileIndex());
		});

		if (getTileIndex() == 0) {
			auto tile = arr.back();
			arr.pop_back();
			arr.insert(arr.begin(), tile);
		}
		return arr;
	}


	template<class T>
	inline std::vector<T*> Hex<T>::getInnerNeighbours()
	{
		auto arr = std::vector<T*>();
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
}