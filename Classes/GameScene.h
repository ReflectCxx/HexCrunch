#pragma once

#include "cocos2d.h"

namespace hex
{
    class GameScene : public cocos2d::Scene
    {
        bool init() override;

    public:

        CREATE_FUNC(GameScene);
    };
}