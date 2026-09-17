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
		HexGrid* m_grid = nullptr;
		GameScene* m_scene = nullptr;

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
		void loadLevel(const HexRingMatrix&);
		bool gridSanityCheck();
		void seedSpawningColors();

		static Game& instance();
		friend GameScene;
	};
}