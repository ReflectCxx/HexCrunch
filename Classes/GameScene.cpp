
#include "Constants.h"
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

        auto bg = Sprite::create(GAME_BG);
        const auto scaleX = SCR_WIDTH / bg->getContentSize().width;
        const auto scaleY = SCR_HEIGHT / bg->getContentSize().height;
        bg->setScale(scaleX, scaleY);
        bg->setPosition({ SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f });
        addChild(bg);

        bg->setOpacity(255 * 0.95f);

        auto grid = HexGrid::create();
        grid->setPosition({ SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f });
        addChild(grid);
        return true;
    }
}