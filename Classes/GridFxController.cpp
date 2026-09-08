
#include "Slider.h"
#include "GridFxController.h"
#include "ActionRotateGrid.h"
#include "ActionSwapTile.h"

namespace hex
{
	void GridFxController::update()
	{
		CommandController::update();
		while (true) {
			const auto command = nextCmd();
			if (command) {
				command->get().execute();
			}
			else break;
		}
	}
}



namespace hex
{
	void GridFxController::pushCb(const std::function<void()>& pCallBack)
	{
		push(Command{ 
			true,
			CmdKind::CallBack, 
			*this,
			[=](Command& pCmd) {
				pCmd.end();
				pCallBack();
			}
		});
	}
}



namespace hex
{
	void GridFxController::rotateGrid(const float pAngle)
	{
		auto cmd = ActionRotateGrid::create(pAngle);
		push(std::move(cmd));
	}


	void GridFxController::swapTiles(const Slider& pSlider, const std::function<void()>& pOnEndCb)
	{
		auto cmd = ActionSwapTile::create(pSlider.actor(), pSlider.follower(), true);
		push(std::move(cmd));
		pushCb(pOnEndCb);
	}


	void GridFxController::swapTiles(HexTile& pTileA, HexTile& pTileB, const std::function<void()>& pOnEndCb)
	{
		auto cmd = ActionSwapTile::create(pTileA, pTileB, false);
		push(std::move(cmd));
		pushCb(pOnEndCb);
	}
}