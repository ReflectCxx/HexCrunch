
#include "Game.h"
#include "Slider.h"
#include "HexGrid.h"
#include "GridFxController.h"

#include "CmdPullSwap.h"
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
	void GridFxController::pushAcquireNeighbour(HexTile& pTile, HexTile& pPullFrom)
	{
		auto cmd = CmdPullSwap::create(pTile, pPullFrom);
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


	void GridFxController::pushCallback(const CallBack& pCallBack)
	{
		push(Command{
			BlocksQ::Join, *this,
			[=](Command& pCmd)->void {
				pCmd.end();
				pCallBack();
			}
		});
	}


	void GridFxController::pushRotateGrid(const float pAngle, const CallBack& pCallBack)
	{
		push(Command{
			BlocksQ::Join,
			Game::instance().fxController(),
			[=](Command& pCmd)->void {

				Game::instance().grid().runAction(
					cocos2d::Sequence::create(
						cocos2d::RotateBy::create(0.5f, pAngle),
						cocos2d::CallFunc::create(
						[&]()->void {
							pCmd.end();
							pCallBack();
						}), nullptr
					)
				);
			}
		});
	}


	void GridFxController::pushSpawnTile(HexTile& pTile)
	{
		push(Command{
			BlocksQ::No, *this,
			[&](Command& pCmd)->void {

				pTile.assignColor(pTile.getSpawnColor());
				pTile.setSpawnColor(ColorId::None);
				pTile.refreshView();
				pTile.setScale(0.01f);
				pTile.setState(TileState::Idle);

				const auto ms = cocos2d::RandomHelper::random_int(250, 1000);
				pTile.runAction(
					cocos2d::Sequence::create(
						cocos2d::ScaleTo::create(ms / 1000.f, 1.f),
						cocos2d::CallFunc::create(
							[&]()->void { 
								pCmd.end(); 
							}), nullptr
					)
				);
			}
		});
	}
}