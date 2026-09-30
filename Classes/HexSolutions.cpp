
#include <array>

#include "Game.h"
#include "HexTile.h"
#include "HexSolutions.h"

namespace
{
	constexpr auto COLOR_N = hex::RING_COUNT;
	constexpr auto SECTOR_CN = COLOR_N * (COLOR_N + 1) / 2;
	constexpr auto EXCLUDE_C = hex::ColorId::Blue;
	constexpr std::array<hex::ColorId, COLOR_N> COLORS = {

		hex::ColorId::Red,
		hex::ColorId::Blue,
		hex::ColorId::Green,
		hex::ColorId::Purple,
		hex::ColorId::Yellow
	};
}


namespace hex
{
    using ColorMap = std::map<ColorId, int>;
    using SectorColorMap = std::array<ColorMap, HEX_6>;
    using SectorTileCount = std::array<int, HEX_6>;


    static int getCount(const ColorMap& pColors, ColorId pColor)
    {
        const auto it = pColors.find(pColor);
        return (it != pColors.end() ? it->second : 0);
    }


    static void extractStrayState(const SectorColorMap& pStrayColors,
                                  ColorMap& pGlobalColors, SectorTileCount& pStrayTiles)
    {
        for (int sector = 0; sector < HEX_6; sector++) {
            for (const auto& [color, count] : pStrayColors[sector]) {
                pGlobalColors[color] += count;
                pStrayTiles[sector] += count;
            }
        }
    }


    static std::vector<ColorId> sortByCount(const ColorMap& pGlobalColors,
                                            const std::array<bool, 5>* pAllowed = nullptr)
    {
        std::vector<ColorId> result;
        for (std::size_t i = 0; i < COLORS.size(); ++i) {
            if (!pAllowed || (*pAllowed)[i])
                result.push_back(COLORS[i]);
        }

        std::sort(result.begin(), result.end(),
            [&pGlobalColors](ColorId lhs, ColorId rhs) {
                return (getCount(pGlobalColors, lhs) > getCount(pGlobalColors, rhs));
            }
        );
        return result;
    }


    // ------------------------------------------------------------
    // PASS 1
    //
    // Fix existing idle imbalance.
    // Larger global pools get priority.
    // ------------------------------------------------------------
    static std::array<bool, 5> balanceIdleColors(const SectorColorMap& pIdle, SectorColorMap& pAssignment,
                                                 SectorTileCount& pRemainingTiles, ColorMap& pGlobalColors)
    {
        std::array<bool, 5> balanced{};
        const auto order = sortByCount(pGlobalColors);

        for (const ColorId color : order)
        {
            int target = 0;
            for (int sector = 0; sector < HEX_6; ++sector) {
                target = std::max(target, getCount(pIdle[sector], color));
            }

            std::array<int, HEX_6> needed{};
            int totalNeeded = 0;
            bool feasible = true;

            for (int sector = 0; sector < HEX_6; ++sector)
            {
                needed[sector] = (target - getCount(pIdle[sector], color));
                if (needed[sector] > pRemainingTiles[sector]) {
                    feasible = false;
                    break;
                }
                totalNeeded += needed[sector];
            }

            const auto colorIt = std::find(COLORS.begin(), COLORS.end(), color);
            const int colorIndex = static_cast<int>(std::distance(COLORS.begin(), colorIt));
            // Already balanced.
            if (totalNeeded == 0) {
                balanced[colorIndex] = true;
                continue;
            }

            // Must completely balance or leave untouched.
            if (!feasible || totalNeeded > getCount(pGlobalColors, color)) {
                continue;
            }

            for (int sector = 0; sector < HEX_6; ++sector) {
                if (needed[sector] == 0) {
                    continue;
                }
                pAssignment[sector][color] += needed[sector];
                pRemainingTiles[sector] -= needed[sector];
            }

            pGlobalColors[color] -= totalNeeded;
            balanced[colorIndex] = true;
            assert(pGlobalColors[color] % HEX_6 == 0);
        }
        return balanced;
    }


    // ------------------------------------------------------------
    // PASS 2
    //
    // Fill every sector uniformly with remaining balanced colors.
    // Larger pools first.
    // ------------------------------------------------------------
    static void fillUniformLayers(SectorColorMap& pAssignment, SectorTileCount& pRemainingTiles,
                                  ColorMap& pGlobalColors, const std::array<bool, 5>& pBalanced)
    {
        int commonCapacity = *std::min_element(pRemainingTiles.begin(), pRemainingTiles.end());
        if (commonCapacity <= 0) {
            return;
        }

        const auto order = sortByCount(pGlobalColors, &pBalanced);
        for (const ColorId color : order)
        {
            if (commonCapacity <= 0) {
                break;
            }

            const int available = getCount(pGlobalColors, color);
            if (available <= 0) {
                continue;
            }
            assert(available % HEX_6 == 0);

            const int uniformCount = std::min(available / HEX_6, commonCapacity);
            if (uniformCount <= 0) {
                continue;
            }

            for (int sector = 0; sector < HEX_6; ++sector)
            {
                pAssignment[sector][color] += uniformCount;
                pRemainingTiles[sector] -= uniformCount;
                assert(pRemainingTiles[sector] >= 0);
            }

            pGlobalColors[color] -= (uniformCount * HEX_6);
            commonCapacity -= uniformCount;
        }
    }


    static std::string buildRemainingLog(const ColorMap& pGlobalColors, const SectorTileCount& pRemainingTiles)
    {
        auto colorName = [](ColorId color) {
            switch (color) {
            case ColorId::Red:    return "R";
            case ColorId::Green:  return "G";
            case ColorId::Blue:   return "B";
            case ColorId::Yellow: return "Y";
            case ColorId::Purple: return "P";
            default: return "?";
            }
        };


        std::stringstream ss;
        ss << "Remaining global stray colors:\n";
        for (const ColorId color : COLORS) {
            ss << colorName(color) << "(" << getCount(pGlobalColors, color) << ") ";
        }


        ss << "\nRemaining stray tiles per sector:\n";
        for (int sector = 0; sector < HEX_6; ++sector) {
            ss << "S" << (sector + 1) << "(" << pRemainingTiles[sector] << ") ";
        }
        return ss.str();
    }
}



namespace hex
{
    void HexSolutions::log(SectorColors sector)
    {
        std::ostringstream ss;
        for (int si = 0; si < HEX_6; ++si) {
            ss << "\nS" << (si + 1)
                << " : R(" << sector[si][ColorId::Red] << ")"
                << " G(" << sector[si][ColorId::Green] << ")"
                << " B(" << sector[si][ColorId::Blue] << ")"
                << " Y(" << sector[si][ColorId::Yellow] << ")"
                << " P(" << sector[si][ColorId::Purple] << ")";
        }
        CCLOG("%s", ss.str().c_str());
    }


	const SectorColors HexSolutions::getSectorColors() const
	{
		SectorColors sector = {};
		for (int si = 0; si < HEX_6; si++) {
			for (const auto color : COLORS) {
				sector[si][color] = 0;
			}
		}

		for (int si = 0; si < HEX_6; si++) {
			for (int ri = 0; ri < RING_COUNT; ri++)
			{
				const int tn = ri + 1;
				const int t0 = si * tn;
				for (int ti = 0; ti < tn; ti++) 
				{
					const auto& t = *m_hexRings[ri][t0 + ti];
					const auto color = t.getColorId();
					if (color != ColorId::None) {
						sector[si][color]++;
					}
				}
			}
		}
		return sector;
	}


	const SectorColors HexSolutions::getSectorStrayColors() const
	{
		SectorColors sectorStrays = {};
		for (int si = 0; si < HEX_6; si++) {
			for (const auto color : COLORS) {
				sectorStrays[si][color] = 0;
			}
		}

		for (int si = 0; si < HEX_6; si++) {
			for (int ri = 0; ri < RING_COUNT; ri++)
			{
				const int tn = ri + 1;
				const int t0 = si * tn;
				for (int ti = 0; ti < tn; ti++)
				{
					const auto& t = *m_hexRings[ri][t0 + ti];
					if (t.getState() == TileState::Stray) {
						const auto color = t.getSpawnColor();
						sectorStrays[si][color]++;
					}
				}
			}
		}
		return sectorStrays;
	}

    
    void HexSolutions::seedColors(const std::vector<int>& pSectorColorN) const
    {
        for (int si = 0; si < HEX_6; si++)
        {
            std::vector<ColorId> sector;
            sector.reserve(SECTOR_CN);
            for (int ri = 0; ri < RING_COUNT; ri++) {
                for (int n = 0; n < pSectorColorN[ri]; n++) {
                    sector.push_back(COLORS[ri]);
                }
            }

            std::shuffle(sector.begin(), sector.end(), Game::rng());

            int ci = 0;
            for (int ri = RING_COUNT - 1; ri >= 0; ri--) {
                const int tn = ri + 1;
                const int t0 = si * tn;
                for (int i = 0; i < tn; ++i) {
                    const auto tile = m_hexRings[ri][t0 + i];
                    tile->assignColor(sector[ci++]);
                }
            }
        }
    }


    void HexSolutions::seedSpawnColors(const SectorColors& pPerSectorStrayColorN) const
    {
        for (int si = 0; si < HEX_6; si++)
        {
            std::vector<ColorId> sector;
            sector.reserve(SECTOR_CN);
            for (const auto color : COLORS) {
                const auto it = pPerSectorStrayColorN[si].find(color);
                if (it == pPerSectorStrayColorN[si].end()) {
                    continue;
                }
                for (int n = 0; n < it->second; ++n) {
                    sector.push_back(color);
                }
            }

            std::size_t ci = 0;
            for (int ri = RING_COUNT - 1; ri >= 0; ri--)
            {
                const int tn = ri + 1;
                const int t0 = si * tn;
                for (int i = 0; i < tn; ++i)
                {
                    auto* tile = m_hexRings[ri][t0 + i];
                    if (tile->getState() != TileState::Stray) {
                        continue;
                    }
                    if (tile->getSpawnColor() != ColorId::None) {
                        continue;
                    }
                    if (ci >= sector.size()) {
                        continue;
                    }
                    tile->setSpawnColor(sector[ci++]);
                }
            }
            assert(ci == sector.size());
        }
    }


	void HexSolutions::balanceStrayColors() const
	{
        // Never modified.
        const SectorColorMap perSectorIdleColorN = getSectorColors();
        SectorColorMap perSectorStrayColorN = getSectorStrayColors();

        CCLOG("Current perSectorIdleColorN:");
        log(perSectorIdleColorN);

        CCLOG("Current perSectorStrayColorN:");
        log(perSectorStrayColorN);

        ColorMap globalStrayColorN;
        SectorTileCount perSectorStrayTileN{};
        extractStrayState(perSectorStrayColorN, globalStrayColorN, perSectorStrayTileN);

        // Original stray-sector color ownership no longer matters.
        // Reuse this as the result.
        for (auto& sector : perSectorStrayColorN) {
            sector.clear();
        }

        const auto balanced = balanceIdleColors(perSectorIdleColorN, perSectorStrayColorN, perSectorStrayTileN, globalStrayColorN);

        fillUniformLayers(perSectorStrayColorN, perSectorStrayTileN, globalStrayColorN, balanced);

        // Final conservation check.
        int remainingTiles = 0;
        int remainingColors = 0;
        for (const int count : perSectorStrayTileN) {
            assert(count >= 0);
            remainingTiles += count;
        }

        for (const ColorId color : COLORS) {
            const int count = getCount(globalStrayColorN, color);
            assert(count >= 0);
            remainingColors += count;
        }
        assert(remainingTiles == remainingColors);
        CCLOG("%s", buildRemainingLog(globalStrayColorN, perSectorStrayTileN).c_str());
        log(perSectorStrayColorN);

        for (int i = 0; i < RING_COUNT; i++) {
            for (auto tile : m_hexRings[i]) {
                if (tile->getState() == TileState::Stray) {
                    tile->setSpawnColor(ColorId::None);
                }
            }
        }

        seedSpawnColors(perSectorStrayColorN);
        for (auto& sector : perSectorStrayColorN) {
            sector.clear();
        }

        for (auto& [color, count] : globalStrayColorN)
        {
            while (count > 0) {
                bool assigned = false;
                for (int si = 0; si < HEX_6 && count > 0; ++si) {
                    if (perSectorStrayTileN[si] <= 0)
                        continue;

                    ++perSectorStrayColorN[si][color];
                    --perSectorStrayTileN[si];
                    --count;
                    assigned = true;
                }
                if (!assigned) {
                    break;
                }
            }
        }

        // Sanity check here.
        for (const auto& [color, count] : globalStrayColorN) {
            assert(count == 0);
        }

        for (const int count : perSectorStrayTileN) {
            assert(count == 0);
        }

        seedSpawnColors(perSectorStrayColorN);
	}
}