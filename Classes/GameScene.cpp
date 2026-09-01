
#include "Constants.h"
#include "HexGrid.h"
#include "GameScene.h"

USING_NS_CC;

namespace hex 
{
    void GameScene::initMenuButtons()
    {
        auto closeItem = MenuItemImage::create(BTN_EXIT_NORMAL, BTN_EXIT_SELECTED,
            [this](cocos2d::Ref*) {
                Director::getInstance()->end();
            });

        const auto x = (SCR_WIDTH - closeItem->getContentSize().width / 2.f);
        const auto y = (closeItem->getContentSize().height / 2.f);
        closeItem->setPosition({ x, y });

        auto menu = Menu::create(closeItem, nullptr);
        menu->setPosition(Vec2::ZERO);
        addChild(menu);
    }


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
        grid->setPosition({ SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f + GRID_HEIGHT / 6.f });
        addChild(grid);

        initMenuButtons();
        return true;
    }
}