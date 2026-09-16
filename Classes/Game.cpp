

#include "HexGrid.h"
#include "HexTile.hpp"
#include "GridManager.h"
#include "Game.h"


namespace hex
{
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

	void Game::loadLevel(std::deque<ColorId>& pColorQ) {
		m_hexSols.seedColors(pColorQ);
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

	Game& Game::instance()
	{
		static Game _instance;
		return _instance;
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
}