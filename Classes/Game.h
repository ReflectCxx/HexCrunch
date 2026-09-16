#pragma once

#include "Constants.h"
#include "HexSolutions.h"

namespace hex
{
	class HexGrid;
	class GameScene;
	class GridManager;
	class GridFxController;

	class Game
	{
		std::vector<std::pair<ColorId, int>> m_colors;
		std::vector<std::pair<ColorId, int>> m_offGrid;

		HexGrid* m_grid = nullptr;
		GameScene* m_scene = nullptr;
		HexSolutions m_hexSols = {};

		Game() = default;
		void setGrid(HexGrid* pGrid);
		void setGameScene(GameScene*);

	public:

		Game(Game&&) = delete;
		Game(const Game&) = delete;
		Game& operator=(Game&&) = delete;
		Game& operator=(const Game&) = delete;

		HexGrid& grid();
		GameScene& scene();
		GridManager& gridManager();
		GridFxController& fxController();

		bool acceptInput();
		bool gridSanityCheck();
		void loadLevel(std::deque<ColorId>&);

		static Game& instance();
		friend GameScene;
	};
}