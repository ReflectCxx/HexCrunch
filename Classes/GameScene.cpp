
#include "Constants.h"
#include "Asset.h"
#include "HexGrid.h"
#include "GameScene.h"

USING_NS_CC;

namespace hex 
{
    bool GameScene::init()
    {
        if (!Scene::init()) {
            return false;
        }
        addChild(Asset::createGameBg());

        auto grid = HexGrid::create();
        grid->setPosition({ SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f + GRID_HEIGHT / 6.f });
        addChild(grid);

        addChild(Asset::createExitBtn([this](Ref*) {
            Director::getInstance()->end();
        }));
        return true;
    }
}