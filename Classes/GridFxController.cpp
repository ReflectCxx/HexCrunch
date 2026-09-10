
#include "Game.h"
#include "Slider.h"
#include "HexGrid.h"
#include "GridFxController.h"

#include "CmdPullSwap.h"
#include "CmdRotateGrid.h"
#include "CmdSliderSwap.h"
#include "CmdClearRingTile.h"

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
	void GridFxController::pushRotateGrid(const float pAngle)
	{
		auto cmd = CmdRotateGrid::create(pAngle);
		push(std::move(cmd));
	}


	void GridFxController::pushSliderSwap(const Slider& pSlider, const CallBack& pOnEndCb)
	{
		auto cmd = CmdSliderSwap::create(pSlider.actor(), pSlider.follower());
		push(std::move(cmd));
		pushCallback(pOnEndCb);
	}


	void GridFxController::pushClearRing(const int pIndex)
	{
		auto& ring = Game::instance().grid().getHexagonRings().at(pIndex);
		for (const auto tile : ring) {
			push(CmdClearRingTile::create(*tile));
		}
	}


	bool GridFxController::pushAcquireNeighbour(HexTile& pTile)
	{
		auto cmd = CmdPullSwap::create(pTile);
		if (cmd) {
			push(std::move(cmd.value()));
			return true;
		}
		return false;
	}


	void GridFxController::pushCallback(const CallBack& pCallBack)
	{
		push(Command{
			BlocksQ::Join,
			CmdKind::CallBack,
			*this, [=](Command& pCmd)->void {
				pCmd.end();
				pCallBack();
			}
		});
	}
}