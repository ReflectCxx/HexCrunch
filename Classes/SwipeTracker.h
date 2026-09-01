#pragma once

#include <functional>

#include "cocos2d.h"
#include "Constants.h"

namespace hex
{
    struct SwipeTracker
    {
        using Callback = std::function<void(Slide)>;

        bool m_isSwiping = false;

        std::chrono::steady_clock::time_point m_lastTapTime;

        ~SwipeTracker();

        SwipeTracker(cocos2d::Node* target, Callback callback);

    private:

        void init();

        Callback m_callback;
        cocos2d::Vec2 m_startPos;
        cocos2d::Node* m_target = nullptr;
        cocos2d::EventListenerTouchOneByOne* m_listener = nullptr;
    };
}