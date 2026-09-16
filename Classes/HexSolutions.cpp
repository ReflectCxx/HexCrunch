
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

	std::mt19937 g_rng{ std::random_device{}() };
}


namespace hex
{
	void HexSolutions::scanSectors(const HexRingMatrix& pGridMat)
	{
		std::array<std::map<ColorId, int>, HEX_6> sectorColors;

		for (int si = 0; si < HEX_6; ++si)
		{
			for (int ri = 0; ri < RING_COUNT; ++ri)
			{
				const int tilesN = ri + 1;
				const int t0 = si * tilesN;

				for (int ti = 0; ti < tilesN; ++ti)
				{
					const auto t = pGridMat[ri][t0 + ti];
					const auto col = t->getColorId();
					sectorColors[si][col]++;
				}
			}
		}
	}


	void HexSolutions::seedColors(std::deque<ColorId>& pColorQ)
	{
		std::array<std::vector<ColorId>, RING_COUNT> rings;
		for (int ri = 0; ri < RING_COUNT; ri++) {
			rings[ri].resize(HEX_6 * (ri + 1));
		}

		auto seq = std::vector<int>{ 1, 2, 3, 4, 5 };
		std::shuffle(seq.begin(), seq.end(), g_rng);

		for (int si = 0; si < HEX_6; si++)
		{
			std::vector<ColorId> sectorColors;
			sectorColors.reserve(SECTOR_CN);
			for (int ri = 0; ri < RING_COUNT; ri++) {
				for (int n = 0; n < seq[ri]; n++) {
					sectorColors.push_back(COLORS[ri]);
				}
			}

			int ci = 0;
			for (int ri = 0; ri < RING_COUNT; ++ri)
			{
				const int tilesN = ri + 1;
				const int t0 = si * tilesN;
				for (int i = 0; i < tilesN; ++i) {
					rings[ri][t0 + i] = sectorColors[ci++];
				}
			}
		}

		for (int ri = 0; ri < RING_COUNT; ri++) {
			for (ColorId color : rings[ri]) {
				pColorQ.push_back(color);
			}
		}
	}
}