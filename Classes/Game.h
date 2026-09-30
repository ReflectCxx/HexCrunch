#pragma once

#include <random>

#include "Constants.h"
#include "TouchTracker.h"
#include "TouchConsumer.h"

class AppDelegate;

namespace hex
{
	class HexGrid;
	class GameScene;
	class GridManager;
	class TouchTracker;
	class GridFxController;

	class Game
	{
		HexGrid* m_grid = nullptr;

		GameScene* m_scene = nullptr;

		TouchConsumer m_touchConsumer = {};

		std::vector<int> m_sectorColorsN = { 7, 1, 5, 1, 1 };

		std::unique_ptr<TouchTracker> m_touchTracker = nullptr;

		Game() = default;

		bool init();
		
	public:

		Game(Game&&) = delete;
		Game(const Game&) = delete;
		Game& operator=(Game&&) = delete;
		Game& operator=(const Game&) = delete;

		HexGrid& grid() const;
		GameScene& scene() const;
		GridManager& gridManager() const;
		TouchConsumer& touch();
		GridFxController& fxController() const;

		bool acceptInput();
		bool gridSanityCheck();
		void seedSpawningColors() const;

		static Game& instance();
		static std::mt19937& rng();

		friend AppDelegate;
	};
}