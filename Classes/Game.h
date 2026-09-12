#pragma once

#include "Constants.h"

namespace hex
{
	class HexGrid;
	class GameScene;
	class GridManager;
	class GridFxController;

	class Game
	{		
		std::vector<ColorId> m_colorStack;
		std::vector<std::pair<ColorId, int>> m_colors;
		std::vector<std::pair<ColorId, int>> m_offGrid;

		HexGrid* m_grid = nullptr;

		Game();
		void seedColors();
		void setGrid(HexGrid* pGrid);
		void pushColor(const ColorId);

	public:

		Game(Game&&) = delete;
		Game(const Game&) = delete;
		Game& operator=(Game&&) = delete;
		Game& operator=(const Game&) = delete;

		HexGrid& grid();
		GridManager& gridManager();
		GridFxController& fxController();

		bool acceptInput();
		const ColorId popColor();
		void ringCleared(const std::size_t pRingIndex);

		static Game& instance();
		friend GameScene;
	};
}