
#include "GameScene.h"
#include "Game.h"

#include "Asset.h"
#include "HexGrid.h"


USING_NS_CC;

namespace hex
{
    GameScene::GameScene() = default;
    GameScene::~GameScene() = default;

    void GameScene::showText(const std::string& pStr)
    {
        m_text->setString(pStr);
        const auto& sz = m_text->getContentSize();
        m_text->setPosition({ SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f - (2 * sz.height) });
    }

    bool GameScene::init()
    {
        if (!Scene::init()) {
            return false;
        }
        addChild(Asset::createGameBg());

        const auto grid = HexGrid::create();
        grid->setPosition({ SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f + GRID_HEIGHT / 6.f });
        addChild(grid);
        
        Game::instance().setGrid(grid);
        Game::instance().setGameScene(this);

        m_touchConsumer = std::make_unique<TouchConsumer>();
        m_touchConsumer->init(grid->getHexagonRings()[RING_COUNT - 2][0]);

        m_touchTracker = std::make_unique<TouchTracker>(
            this,
            [tc = m_touchConsumer.get()](Swipe pDir) {
                tc->onInputRecieved(pDir);
            }
        );

        m_text = Label::createWithTTF("", FONT, 50.f);
        addChild(m_text);

        addChild(Asset::createExitBtn([this](Ref*) {
            Director::getInstance()->end();
        }));

        Game::instance().gridSanityCheck();
        return true;
    }
}