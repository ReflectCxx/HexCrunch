
#include "HexGrid.h"
#include "GridManager.h"
#include "Game.h"

namespace hex
{
	Game::Game()
		:m_grid(HexGrid::create()) 
	{ }

	HexGrid& Game::grid() {
		return (*m_grid);
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