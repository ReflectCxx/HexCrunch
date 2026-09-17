
#include "Game.h"
#include "GameScene.h"

#include "HexGrid.h"
#include "HexTile.hpp"
#include "HexSolutions.h"
#include "GridManager.h"


namespace hex
{
	Game& Game::instance() {
		static Game _;
		return _;
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

	bool Game::acceptInput() {
		return (m_grid->manager().controller().getRunningCmdCount() == 0);
	}
}


namespace hex
{
	bool Game::gridSanityCheck()
	{
		auto& hexRings = m_grid->getHexagonRings();
		const auto str = HexSolutions{ hexRings }.scanSectors();
		m_scene->showText(str);
		CCLOG("%s", str.c_str());
		return true;
	}


	void Game::loadLevel(const HexRingMatrix& pRings)
	{
		std::deque<ColorId> colorQ;
		HexSolutions::seedColors(colorQ);

		for (int ri = 0; ri < RING_COUNT; ri++) {
			for (const auto t : pRings[ri]) {
				const auto color = colorQ.front();
				colorQ.pop_front();
				t->assignColor(color);
			}
		}
	}
}