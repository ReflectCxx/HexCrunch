
#include <array>
#include <random>

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

	std::mt19937 g_rng{ std::random_device{}() };
}


namespace hex
{
	void HexSolutions::seedColors(std::deque<ColorId>& pColorQ)
	{
		std::array<std::vector<ColorId>, RING_COUNT> rings;
		for (int r = 0; r < RING_COUNT; ++r) {
			rings[r].resize(HEX_6 * (r + 1));
		}

		auto seq = std::vector<int>{ 1, 2, 3, 4, 5 };
		std::shuffle(seq.begin(), seq.end(), g_rng);

		for (int sector = 0; sector < HEX_6; ++sector)
		{
			std::vector<ColorId> sectorColors;
			sectorColors.reserve(SECTOR_CN);
			for (int ri = 0; ri < RING_COUNT; ri++) {
				for (int n = 0; n < seq[ri]; n++) {
					sectorColors.push_back(COLORS[ri]);
				}
			}

			int src = 0;
			for (int ring = 0; ring < RING_COUNT; ++ring)
			{
				const int sectorWidth = ring + 1;
				const int startTile = sector * sectorWidth;
				for (int i = 0; i < sectorWidth; ++i) {
					rings[ring][startTile + i] = sectorColors[src++];
				}
			}
		}

		for (int ring = 0; ring < RING_COUNT; ++ring) {
			for (ColorId color : rings[ring]) {
				pColorQ.push_back(color);
			}
		}
	}
}