#pragma once

#include <functional>

#include "cocos2d.h"
#include "Constants.h"

namespace hex
{
    class TouchTracker
    {
        bool m_isSwiping = false;

        std::function<void(Swipe)> m_callback;
        std::chrono::steady_clock::time_point m_lastTapTime;

        cocos2d::Vec2 m_startPos;
        cocos2d::Node* m_target = nullptr;
        cocos2d::EventListenerTouchOneByOne* m_listener = nullptr;

        void init();

    public:

        ~TouchTracker();
        TouchTracker(cocos2d::Node* pTarget, const std::function<void(Swipe)>& pCallback);
    };
}