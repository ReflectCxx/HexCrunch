
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
		const auto color = m_colorStack.back();
		m_colorStack.pop_back();
		return color;
	}

	void Game::ringCleared(const std::size_t pRingIndex)
	{
		const auto color = m_grid->getHexagonRings()[pRingIndex][0]->getColorId();
		const auto popCount = (HEX_6 * (pRingIndex + 1));

		for (int i = 0; i < popCount; i++) {
			pushColor(color);
		}
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

		for (int i = 0; i < m_colors.size(); i++) {
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
		static std::mt19937 s_rng{ std::random_device{}() };

		std::vector<int> count = { 6, 12, 18, 24, 30 };		//total tiles 90 tiles.
		const std::vector<hex::ColorId> colors = {
			hex::ColorId::Red,
			hex::ColorId::Green,
			hex::ColorId::Yellow,
			hex::ColorId::Blue,
			hex::ColorId::Purple
		};

		std::shuffle(count.begin(), count.end(), s_rng);
		for (std::size_t i = 0; i < count.size(); i++) {
			m_colors.push_back({ colors[i], count[i] });
			m_offGrid.push_back({ colors[i], 0 });
		}

		for (size_t i = 0; i < colors.size(); ++i) {
			m_colorStack.insert(m_colorStack.end(), count[i], colors[i]);
		}
		std::shuffle(m_colorStack.begin(), m_colorStack.end(), s_rng);
	}
}