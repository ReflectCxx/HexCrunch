
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


	bool Game::gridSanityCheck()
	{
		auto& hexRings = m_grid->getHexagonRings();
		auto sector = HexSolutions{ hexRings }.getSectorColors();

		std::ostringstream ss;
		for (int si = 0; si < HEX_6; ++si) {
			ss << "\nS" << (si + 1)
				<< " : R(" << sector[si][ColorId::Red] << ")"
				<< " G(" << sector[si][ColorId::Green] << ")"
				<< " B(" << sector[si][ColorId::Blue] << ")"
				<< " Y(" << sector[si][ColorId::Yellow] << ")"
				<< " P(" << sector[si][ColorId::Purple] << ")";
		}

		m_scene->showText(ss.str());
		CCLOG("%s", ss.str().c_str());
		return true;
	}


	void Game::seedSpawningColors()
	{
		auto& hexRings = Game::instance().grid().getHexagonRings();
		auto sectorAssign = HexSolutions{ hexRings }.seedSectorSpawns();
		for (int si = 0; si < HEX_6; si++)
		{
			for (int ri = 0; ri < RING_COUNT; ri++)
			{
				const int tn = ri + 1;
				const int t0 = si * tn;
				for (int ti = 0; ti < tn; ++ti) {
					auto& t = *hexRings[ri][t0 + ti];
					if (t.getColorId() == ColorId::None && t.getSpawnColor() == ColorId::None && !sectorAssign[si].empty()) {
						t.setSpawnColor(sectorAssign[si].back());
						sectorAssign[si].pop_back();
					}
				}
			}
			assert(sectorAssign[si].empty());
		}
	}
}