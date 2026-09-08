
#include "Slider.h"
#include "GridFxController.h"
#include "CmdRotateGrid.h"
#include "CmdSliderSwap.h"

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


	void GridFxController::pushCb(const std::function<void()>& pCallBack)
	{
		push(Command{
			BlocksQ::Yes,
			CmdKind::CallBack,
			*this, [=](Command& pCmd)->void {
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
		auto cmd = CmdRotateGrid::create(pAngle);
		push(std::move(cmd));
	}


	void GridFxController::swapTiles(const Slider& pSlider, const std::function<void()>& pOnEndCb)
	{
		auto cmd = CmdSliderSwap::create(pSlider.actor(), pSlider.follower());
		push(std::move(cmd));
		pushCb(pOnEndCb);
	}


	void GridFxController::swapTiles(HexTile& pTileA, HexTile& pTileB)
	{
		//auto cmd = CmdSwapTile::create(pTileA, pTileB, false);
		//push(std::move(cmd));
		//pushCb(pOnEndCb);
	}
}