
#include "Game.h"
#include "HexGrid.h"
#include "GameScene.h"
#include "TouchConsumer.h"


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

	HexGrid& Game::grid() const {
		return (*m_grid);
	}

	GameScene& Game::scene() const {
		return (*m_scene);
	}

	GridManager& Game::gridManager() const {
		return m_grid->manager();
	}

	TouchConsumer& Game::touch() {
		return m_touchConsumer;
	}

	GridFxController& Game::fxController() const {
		return m_grid->manager().controller();
	}

	bool Game::acceptInput() {
		return (m_grid->manager().controller().getRunningCmdCount() == 0);
	}
}


namespace hex
{
	void Game::seedSpawningColors() const
	{
		HexSolutions{
			grid().getHexagonRings()
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


	bool Game::init()
	{
		m_grid = HexGrid::create();
		if (!m_grid) {
			return false;
		}

		auto& hexRings = grid().getHexagonRings();

		//std::shuffle(m_sectorColorsN.begin(), m_sectorColorsN.end(), rng());

		HexSolutions{ hexRings }.seedColors(m_sectorColorsN);

		for (auto& rings : hexRings) {
			for (const auto t : rings) {
				t->setState(TileState::Idle);
			}
		}

		m_scene = GameScene::create();
		if (!m_scene) {
			return false;
		}

		m_scene->setupHexGrid(m_grid);
		m_scene->setDctorCallback([this]() {
			m_touchTracker->deInit();
		});

		m_touchTracker = std::make_unique<TouchTracker>(
			m_scene,
			[this](Swipe pDir) {
				m_touchConsumer.onInputRecieved(pDir);
			}
		);

		m_touchConsumer.init(hexRings[RING_COUNT - 2][0]);
		return true;
	}
}