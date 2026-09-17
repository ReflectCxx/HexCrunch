
#include <array>
#include <random>

#include "HexSolutions.h"
#include "HexTile.h"

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

	static std::mt19937& rng() {
		static std::mt19937 _{ std::random_device{}() };
		return _;
	}
}


namespace hex
{
	const SectorColors HexSolutions::getSectorColors()
	{
		SectorColors sector = {};
		for (int si = 0; si < HEX_6; si++){
			for (int ri = 0; ri < RING_COUNT; ri++)
			{
				const int tn = ri + 1;
				const int t0 = si * tn;
				for (int ti = 0; ti < tn; ++ti) {
					const auto& t = *m_hexRings[ri][t0 + ti];
					const auto col = t.getColorId();
					sector[si][col]++;
				}
			}
		}
		return sector;
	}


	const std::array<int, HEX_6> HexSolutions::getSectorSpawnCount()
	{
		std::array<int, HEX_6> spawnCount = {};
		for (int si = 0; si < HEX_6; si++) {
			for (int ri = 0; ri < RING_COUNT; ri++)
			{
				const int tn = ri + 1;
				const int t0 = si * tn;
				for (int ti = 0; ti < tn; ++ti) {
					const auto& t = *m_hexRings[ri][t0 + ti];
					if (t.getColorId() == ColorId::None &&
						t.getSpawnColor() == ColorId::None) {
						spawnCount[si]++;
					}
				}
			}
		}
		return spawnCount;
	}


	void HexSolutions::seedColors(std::deque<ColorId>& pColorQ)
	{
		std::array<std::vector<ColorId>, RING_COUNT> rings;
		for (int ri = 0; ri < RING_COUNT; ri++) {
			rings[ri].resize(HEX_6 * (ri + 1));
		}

		auto seq = std::vector<int>{ 1, 2, 3, 4, 5 };
		std::shuffle(seq.begin(), seq.end(), rng());

		for (int si = 0; si < HEX_6; si++)
		{
			std::vector<ColorId> sector;
			sector.reserve(SECTOR_CN);
			for (int ri = 0; ri < RING_COUNT; ri++) {
				for (int n = 0; n < seq[ri]; n++) {
					sector.push_back(COLORS[ri]);
				}
			}

			int ci = 0;
			for (int ri = 0; ri < RING_COUNT; ri++) {
				const int tn = ri + 1;
				const int t0 = si * tn;
				for (int i = 0; i < tn; ++i) {
					rings[ri][t0 + i] = sector[ci++];
				}
			}
		}

		for (int ri = 0; ri < RING_COUNT; ri++) {
			for (ColorId color : rings[ri]) {
				pColorQ.push_back(color);
			}
		}
	}


	const std::array<std::vector<ColorId>, HEX_6> HexSolutions::seedSectorSpawns()
	{
		auto sectorColors = getSectorColors();
		auto sectorSpawnCn = getSectorSpawnCount();
		std::array<std::vector<ColorId>, HEX_6> sectorAssign{};

		while (true)
		{
			ColorId bestColor = ColorId::None;
			int bestPresence = -1;
			int bestTarget = 0;

			//
			// Find the most-present color on the grid that can be
			// COMPLETELY balanced across all sectors in this cycle.
			//
			for (const auto color : COLORS)
			{
				if (color == ColorId::None) {
					continue;
				}

				int target = 0;
				int totalPresence = 0;

				// Balance means bringing every sector up to the
				// current maximum sector count for this color.
				for (int s = 0; s < HEX_6; s++) {
					const int count = sectorColors[s][color];
					target = std::max(target, count);
					totalPresence += count;
				}

				bool canBalance = true;
				int totalDeficit = 0;

				for (int s = 0; s < HEX_6; s++)
				{
					const int deficit = (target - sectorColors[s][color]);
					// This sector does not have enough empty slots
					// to bring this color up to target.
					if (deficit > sectorSpawnCn[s]) {
						canBalance = false;
						break;
					}
					totalDeficit += deficit;
				}

				// Ignore colors that are already balanced.
				if (!canBalance || totalDeficit == 0) {
					continue;
				}

				// Priority:
				// heal the color currently most abundant on the grid.
				if (totalPresence > bestPresence) {
					bestColor = color;
					bestPresence = totalPresence;
					bestTarget = target;
				}
			}

			// No additional color can be completely healed
			// using the remaining empty positions.
			if (bestColor == ColorId::None) {
				break;
			}

			// Fully balance the selected color.
			for (int s = 0; s < HEX_6; ++s)
			{
				const int deficit = (bestTarget - sectorColors[s][bestColor]);
				for (int i = 0; i < deficit; ++i) {
					sectorAssign[s].push_back(bestColor);
				}
				sectorColors[s][bestColor] += deficit;
				sectorSpawnCn[s] -= deficit;
			}
		}
		return sectorAssign;
	}
}