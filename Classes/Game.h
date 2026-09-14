#pragma once

#include <deque>

#include "Constants.h"

namespace hex
{
	class HexGrid;
	class GameScene;
	class GridManager;
	class GridFxController;

	class Game
	{		
		std::deque<ColorId> m_colorStack;
		std::vector<std::pair<ColorId, int>> m_colors;
		std::vector<std::pair<ColorId, int>> m_offGrid;

		HexGrid* m_grid = nullptr;
		GameScene* m_scene = nullptr;

		Game();
		void seedColors();
		void setGrid(HexGrid* pGrid);
		void pushColor(const ColorId);
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
		const ColorId popColor();

		bool gridSanityCheck();

		static Game& instance();
		friend GameScene;
	};
}