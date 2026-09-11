
#include <random>
#include <utility>

#include "HexGrid.h"
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

	void Game::setGrid(HexGrid* pGrid) {
		m_grid = pGrid;
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
}


namespace hex
{
	void Game::seedColors()
	{
		static std::mt19937 s_rng{ std::random_device{}() };

		std::vector<int> count = { 6, 12, 18, 24, 30 };		//total tiles 90 tiles.
		std::vector<hex::ColorId> arr = {
			hex::ColorId::Red,
			hex::ColorId::Green,
			hex::ColorId::Yellow,
			hex::ColorId::Blue,
			hex::ColorId::Purple
		};

		std::shuffle(count.begin(), count.end(), s_rng);

		for (std::size_t i = 0; i < count.size(); i++) {
			m_colors.push_back({ arr[i], count[i] });
		}

		for (size_t i = 0; i < arr.size(); ++i) {
			m_colorStack.insert(m_colorStack.end(), count[i], arr[i]);
		}

		std::shuffle(m_colorStack.begin(), m_colorStack.end(), s_rng);
	}
}