#pragma once

#include "cocos2d.h"

namespace hex
{
    class GameScene : public cocos2d::Scene
    {
        bool init() override;

        void initMenuButtons();

    public:

        CREATE_FUNC(GameScene);
    };
}