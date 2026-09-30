#pragma once

#include <functional>

#include "cocos2d.h"

namespace hex
{
    class HexGrid;
    class GameScene : public cocos2d::Scene
    {
        cocos2d::Label* m_text = nullptr;

        std::function<void()> m_dctorCb = nullptr;

    public:

        GameScene();
        ~GameScene();

        void showText(const std::string&);

        void setupHexGrid(HexGrid*);

        bool init() override;

        void setDctorCallback(const std::function<void()>);

        CREATE_FUNC(GameScene);
    };
}