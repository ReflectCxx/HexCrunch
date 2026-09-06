#pragma once

#include "cocos2d.h"

namespace hex
{
    class TouchTracker;
    class TouchConsumer;

    class GameScene : public cocos2d::Scene
    {
        std::unique_ptr<TouchTracker> m_touchTracker = nullptr;
        std::unique_ptr<TouchConsumer> m_touchConsumer = nullptr;

        bool init() override;

    public:

        GameScene();
        ~GameScene();

        CREATE_FUNC(GameScene);
    };
}