
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
	const std::string HexSolutions::scanSectors()
	{
		std::array<std::map<ColorId, int>, HEX_6> sector;

		for (int si = 0; si < HEX_6; si++){
			for (int ri = 0; ri < RING_COUNT; ri++)
			{
				const int tn = ri + 1;
				const int t0 = si * tn;
				for (int ti = 0; ti < tn; ++ti) {
					const auto t = m_hexRings[ri][t0 + ti];
					const auto col = t->getColorId();
					sector[si][col]++;
				}
			}
		}

		std::ostringstream ss;
		for (int si = 0; si < HEX_6; ++si) {
			ss << "\nS" << (si + 1)
				<< " : R(" << sector[si][ColorId::Red] << ")"
				<< " G(" << sector[si][ColorId::Green] << ")"
				<< " B(" << sector[si][ColorId::Blue] << ")"
				<< " Y(" << sector[si][ColorId::Yellow] << ")"
				<< " P(" << sector[si][ColorId::Purple] << ")";
		}
		return ss.str();
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
}