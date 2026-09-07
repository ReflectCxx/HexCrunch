
#include "GameScene.h"

#include "Asset.h"
#include "Slider.h"
#include "HexGrid.h"
#include "TouchTracker.h"
#include "TouchConsumer.h"


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

        auto grid = HexGrid::create();
        grid->setPosition({ SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f + GRID_HEIGHT / 6.f });
        addChild(grid);

        auto cursor = Slider(*grid);

        m_touchConsumer = std::make_unique<TouchConsumer>(cursor);
        m_touchConsumer->init(grid->getHexagonRings()[RING_COUNT - 2][0]);

        m_touchTracker = std::make_unique<TouchTracker>(this,
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