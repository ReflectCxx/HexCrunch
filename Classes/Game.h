#pragma once

#include <random>

#include "Constants.h"

namespace hex
{
	class HexGrid;
	class GameScene;
	class GridManager;
	class TouchConsumer;
	class GridFxController;

	class Game
	{
		HexGrid* m_grid = nullptr;
		GameScene* m_scene = nullptr;
		TouchConsumer* m_touch = nullptr;
		std::vector<int> m_sectorColorsN = { 7, 1, 5, 1, 1 };

		Game() = default;
		void setGrid(HexGrid* pGrid);
		void setGameScene(GameScene*);
		void setTouchConsumer(TouchConsumer*);

	public:

		Game(Game&&) = delete;
		Game(const Game&) = delete;
		Game& operator=(Game&&) = delete;
		Game& operator=(const Game&) = delete;

		HexGrid& grid();
		GameScene& scene();
		GridManager& gridManager();
		TouchConsumer& touch();
		GridFxController& fxController();

		bool acceptInput();
		bool gridSanityCheck();

		void seedSpawningColors() const;
		void loadLevel(const HexRingMatrix&) const;

		static Game& instance();
		static std::mt19937& rng();
		friend GameScene;
	};
}