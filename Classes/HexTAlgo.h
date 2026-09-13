#pragma once


#include <unordered_map>
#include <unordered_set>

#include "Constants.h"

namespace hex
{
	template<class T>
	class HexAlgo
	{
		using ClaimMap = std::unordered_map<T*, T*>;
		using UsedChildren = std::unordered_set<T*>;

		struct MatchResult
		{
			ColorId color = ColorId::None;
			ClaimMap claims;

			size_t count() const {
				return claims.size();
			}
		};


		static bool tryMatch(T* pParent, ColorId pColor, ClaimMap& pClaims, const UsedChildren& pUsedChildren)
		{
			std::unordered_set<T*> visitedChildren;
			return tryReassign(pParent, pColor, pClaims, visitedChildren, pUsedChildren);
		}


		static MatchResult evaluateColor(const std::vector<T*>& pParents, ColorId pColor, const UsedChildren& pUsedChildren)
		{
			MatchResult result = {};
			result.color = pColor;
			result.claims = matchColor(pParents, pColor, pUsedChildren);
			return result;
		}


		static ClaimMap matchColor(const std::vector<T*>& pParents, ColorId pColor, const UsedChildren& pUsedChildren)
		{
			ClaimMap claims;
			for (T* parent : pParents) {
				tryMatch(parent, pColor, claims, pUsedChildren);
			}
			return claims;
		}


		static std::vector<T*> getChildrenOfColor(T* pParent, ColorId pColor, const UsedChildren& pUsedChildren)
		{
			std::vector<T*> result;
			const auto children = pParent->getOuterNeighbours();
			for (T* child : children)
			{
				if (pUsedChildren.find(child) != pUsedChildren.end()) {
					continue;
				}

				if (child->getColorId() == pColor) {
					result.push_back(child);
				}
			}
			return result;
		}


		static std::unordered_set<ColorId> collectAvailableColors(const std::vector<T*>& pParents, 
																  const UsedChildren& pUsedChildren)
		{
			std::unordered_set<ColorId> colors;

			for (T* parent : pParents)
			{
				for (T* child : parent->getOuterNeighbours())
				{
					if (pUsedChildren.find(child) != pUsedChildren.end()) {
						continue;
					}

					if (child->getColorId() != ColorId::None) {
						colors.insert(child->getColorId());
					}
				}
			}

			return colors;
		}


		static bool tryReassign(T* pParent, ColorId pColor, ClaimMap& pClaims,
								std::unordered_set<T*>& pVisitedChildren, const UsedChildren& pUsedChildren)
		{
			for (T* child : getChildrenOfColor(pParent, pColor, pUsedChildren))
			{
				if (!pVisitedChildren.insert(child).second) {
					continue;
				}

				auto it = pClaims.find(child);
				if (it == pClaims.end()) {
					pClaims[child] = pParent;
					return true;
				}

				T* currentParent = it->second;
				if (tryReassign(currentParent, pColor, pClaims, pVisitedChildren, pUsedChildren)) {
					pClaims[child] = pParent;
					return true;
				}
			}
			return false;
		}


		static MatchResult findBestColorMatch(const std::vector<T*>& pParents,
											  const UsedChildren& pUsedChildren)
		{
			std::vector<MatchResult> bestMatches;
			size_t bestCount = 0;

			const auto colors = collectAvailableColors(pParents, pUsedChildren);
			for (ColorId color : colors)
			{
				MatchResult current = evaluateColor(pParents, color, pUsedChildren);
				if (current.count() > bestCount) {
					bestCount = current.count();
					bestMatches.clear();
					bestMatches.push_back(std::move(current));
				}
				else if (current.count() == bestCount) {
					bestMatches.push_back(std::move(current));
				}
			}

			if (bestMatches.empty()) {
				return {};
			}

			const size_t index = std::rand() % bestMatches.size();
			return std::move(bestMatches[index]);
		}


	public:

		static std::vector<std::pair<T*, T*>> claimNeighboursColor(const std::vector<T*>& pParents)
		{
			if (pParents.empty()) {
				return {};
			}

			std::vector<std::pair<T*, T*>> claimed;

			auto parents = pParents;
			UsedChildren usedChildren;

			while (!parents.empty())
			{
				MatchResult bestMatch = findBestColorMatch(parents, usedChildren);
				if (bestMatch.claims.empty()) {
					break;
				}

				for (const auto& [c, p] : bestMatch.claims)
				{
					claimed.emplace_back(p, c);
					usedChildren.insert(c);
					std::erase(parents, p);
				}
			}
			return claimed;
		}
	};
}