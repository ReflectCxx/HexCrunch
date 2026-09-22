
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

	std::mt19937& Game::rng() {
		static std::mt19937 _{ std::random_device{}() };
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

	void Game::setTouchConsumer(TouchConsumer* pTouch) {
		m_touch = pTouch;
	}

	GridManager& Game::gridManager() {
		return m_grid->manager();
	}

	TouchConsumer& Game::touch() {
		return *m_touch;
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
	void Game::loadLevel(const HexRingMatrix& pRings) const
	{
		//std::shuffle(m_sectorColorsN.begin(), m_sectorColorsN.end(), rng());
		HexSolutions{ pRings }.seedColors(m_sectorColorsN);
	}


	void Game::seedSpawningColors() const
	{
		HexSolutions{
			Game::instance().grid().getHexagonRings()
		}.balanceStrayColors();
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
		CCLOG("Sanity Check:- %s", ss.str().c_str());
		return true;
	}
}