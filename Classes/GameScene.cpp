
#include "GameScene.h"
#include "Game.h"

#include "Asset.h"
#include "HexGrid.h"


USING_NS_CC;

namespace hex
{
    GameScene::GameScene() = default;

    GameScene::~GameScene()
    {
        if (m_dctorCb) {
            m_dctorCb();
        }
    }

    
    void GameScene::setDctorCallback(const std::function<void()> pCb)
    {
        m_dctorCb = pCb;
    }


    void GameScene::setupHexGrid(HexGrid* pGrid)
    {
        pGrid->setPosition({ SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f + GRID_HEIGHT / 6.f });
        addChild(pGrid);
    }


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

        m_text = Label::createWithTTF("", FONT, 50.f);
        addChild(m_text);

        addChild(Asset::createExitBtn([this](Ref*) {
            Director::getInstance()->end();
        }));

        return true;
    }
}