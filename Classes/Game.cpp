
#include <deque>
#include <random>
#include <utility>

#include "HexGrid.h"
#include "HexTile.hpp"
#include "GridManager.h"
#include "Game.h"


namespace
{
	constexpr auto N = hex::RING_COUNT;
	constexpr auto HEX_TILES_N = hex::HEX_6 * (N * (N + 1)) / 2;
	constexpr auto EXCLUDE_COLOR = hex::ColorId::Blue;

	static std::vector<std::pair<hex::ColorId, std::size_t>> initQuota(std::mt19937& pRng)
	{
		std::array<std::size_t, N> quota = { 1, 2, 3, 4, 5 };
		std::shuffle(quota.begin(), quota.end(), pRng);

		std::size_t quota1i = 0, quota2i = 0;
		for (std::size_t i = 0; i < quota.size(); ++i) {
			if (quota[i] == 1) quota1i = i;
			if (quota[i] == 2) quota2i = i;
		}

		std::array<hex::ColorId, N> colors = {
			hex::ColorId::Red,
			hex::ColorId::Green,
			hex::ColorId::Yellow,
			hex::ColorId::Blue,
			hex::ColorId::Purple
		};

		if (colors[quota1i] == EXCLUDE_COLOR) {
			colors[quota1i] = colors[quota2i];
		}
		else if (colors[quota2i] == EXCLUDE_COLOR) {
			colors[quota2i] = colors[quota1i];
		}
		else {
			const auto c = colors[quota1i];
			colors[quota1i] = colors[quota2i];

			for (std::size_t i = 0; i < colors.size(); ++i) {
				if (colors[i] == EXCLUDE_COLOR) {
					colors[i] = c;
					break;
				}
			}
		}

		std::vector<std::pair<hex::ColorId, std::size_t>> cQuota;
		cQuota.reserve(N);
		for (std::size_t i = 0; i < N; ++i) {
			cQuota.emplace_back(colors[i], quota[i]);
		}
		return cQuota;
	}
}


namespace hex
{
	Game::Game() {
		seedColors();
	}

	HexGrid& Game::grid() {
		return (*m_grid);
	}

	GameScene& Game::scene() {
		return (*m_scene);
	}

	void Game::setGrid(HexGrid* pGrid) {
		m_grid = pGrid;
	}

	void Game::setGameScene(GameScene* pScene) {
		m_scene = pScene;
	}

	GridManager& Game::gridManager() {
		return m_grid->manager();
	}

	GridFxController& Game::fxController() {
		return m_grid->manager().controller();
	}

	bool Game::acceptInput()
	{
		return (m_grid->manager().controller().getRunningCmdCount() == 0);
	}

	void Game::pushColor(const ColorId pColorId)
	{
		m_colorStack.push_back(pColorId);
	}

	Game& Game::instance()
	{
		static Game _instance;
		return _instance;
	}

	const ColorId Game::popColor()
	{
		const auto color = m_colorStack.front();
		m_colorStack.pop_front();
		return color;
	}
}


namespace hex
{
	bool Game::gridSanityCheck()
	{
		std::unordered_map<ColorId, int> onGrid;
		const auto& ringsMat = m_grid->getHexagonRings();
		for (const auto& ring : ringsMat) {
			for (const auto t : ring) {
				const auto color = t->getColorId();
				if (color != ColorId::None) {
					onGrid[color]++;
				}
			}
		}

		for (std::size_t i = 0; i < m_colors.size(); i++) {
			const auto [color, count] = m_colors[i];
			const auto onGridCount = onGrid[color];
			const auto offGridCount = m_offGrid[i].second;
			if (count != (onGridCount + offGridCount)) {
				return false;
			}
		}
		return true;
	}

	
	void Game::seedColors()
	{
		constexpr auto SECTOR_TILE_COUNT = RING_COUNT * (RING_COUNT + 1) / 2;

		std::array<std::vector<ColorId>, RING_COUNT> rings;
		static std::mt19937 s_rng{ std::random_device{}() };
		const auto cQuota = initQuota(s_rng);

		for (int r = 0; r < RING_COUNT; ++r) {
			rings[r].resize(HEX_6 * (r + 1));
		}
		 
		for (int sector = 0; sector < HEX_6; ++sector)
		{
			std::vector<ColorId> sectorColors;
			sectorColors.reserve(SECTOR_TILE_COUNT);

			for (int ci = 0; ci < RING_COUNT; ++ci) {
				for (std::size_t n = 0; n < cQuota[ci].second; ++n) {
					sectorColors.push_back(cQuota[ci].first);
				}
			}

			//std::shuffle(sectorColors.begin(), sectorColors.end(), s_rng);

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

		m_colorStack.clear();
		for (int ring = 0; ring < RING_COUNT; ++ring) {
			for (ColorId color : rings[ring]) {
				m_colorStack.push_back(color);
			}
		}
	}
}