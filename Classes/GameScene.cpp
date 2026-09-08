
#include "GameScene.h"
#include "Game.h"

#include "Asset.h"
#include "HexGrid.h"


USING_NS_CC;

namespace hex
{
    GameScene::GameScene() = default;
    GameScene::~GameScene() = default;

    bool GameScene::init()
    {
        if (!Scene::init()) {
            return false;
        }
        addChild(Asset::createGameBg());

        auto& grid = Game::instance().grid();
        grid.setPosition({ SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f + GRID_HEIGHT / 6.f });
        addChild(&grid);
        
        m_touchConsumer = std::make_unique<TouchConsumer>();
        m_touchConsumer->init(grid.getHexagonRings()[RING_COUNT - 2][0]);

        m_touchTracker = std::make_unique<TouchTracker>(
            this,
            [tc = m_touchConsumer.get()](Swipe pDir) {
                tc->onInputRecieved(pDir);
            }
        );

        addChild(Asset::createExitBtn([this](Ref*) {
            Director::getInstance()->end();
        }));

        return true;
    }
}