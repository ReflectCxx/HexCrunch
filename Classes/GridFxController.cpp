
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
		push({
			.m_blocksCmdQ = true,
			.m_cmdKind = CmdKind::CallBack,
			.m_controller = *this,
			.m_command = [=](Command& pCmd) {
				pCmd.executionBegins();
				pCallBack();
				pCmd.executionEnds();
				return true;
			}
		});
	}
}


namespace hex
{
	void GridFxController::rotateGrid(const float pAngle)
	{
		push(ActionRotateGrid::create(*this, m_grid, pAngle));
	}

	void GridFxController::swapTiles(HexTile& pTileA, HexTile& pTileB, const std::function<void()>& pOnEndCb)
	{
		push(ActionSwapTile::create(*this, pTileA, pTileB));
		pushCb(pOnEndCb);
	}
}