#pragma once

namespace hex
{
	class HexGrid;
	class GameScene;
	class GridManager;
	class GridFxController;

	class Game
	{
		HexGrid* m_grid = nullptr;

		Game();

	public:

		Game(Game&&) = delete;
		Game(const Game&) = delete;
		Game& operator=(Game&&) = delete;
		Game& operator=(const Game&) = delete;

		HexGrid& grid();
		GridManager& gridManager();
		GridFxController& fxController();

		bool acceptInput();

		static Game& instance();
		friend GameScene;
	};
}