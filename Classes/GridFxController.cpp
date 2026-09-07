
#include "GridFxController.h"
#include "ActionRotateGrid.h"
#include "ActionSwapTile.h"

namespace hex
{
	GridFxController::GridFxController(HexGrid& pGrid)
		: m_grid(pGrid)
	{ }


	void GridFxController::update()
	{
		while (true) 
		{
			const auto command = nextCmd();
			if (command) {
				command->get().execute();
			}
			else break;
		}
	}


	void GridFxController::pushCb(const std::function<void()>& pCallBack)
	{
		auto cmd = Command{
			*this, CmdKind::SwapTiles, true,
			[=](Command& pCmd) {
				pCmd.executionBegins();
				pCallBack();
				pCmd.executionEnds();
			}
		};
		push(std::move(cmd));
	}
}


namespace hex
{
	void GridFxController::rotateGrid(const float pAngle)
	{
		auto cmd = ActionRotateGrid::create(*this, m_grid, pAngle);
		push(std::move(cmd));
	}


	void GridFxController::swapTiles(HexTile& pTileA, HexTile& pTileB, const std::function<void()>& pOnEndCb)
	{
		auto cmd = ActionSwapTile::create(*this, pTileA, pTileB);
		push(std::move(cmd));
		pushCb(pOnEndCb);
	}
}