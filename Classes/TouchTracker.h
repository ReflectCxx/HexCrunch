#pragma once

#include <functional>

#include "cocos2d.h"
#include "Constants.h"

namespace hex
{
    class TouchTracker
    {
        bool m_isSwiping;
        bool m_isTapCandidate;
        std::function<void(Swipe)> m_callback;

        cocos2d::Vec2 m_startPos;
        cocos2d::Node* m_target;
        cocos2d::EventListenerTouchOneByOne* m_listener;
        std::chrono::steady_clock::time_point m_lastSwipeEndTime;

        void init();
        void onTouchBegan(const cocos2d::Vec2& pPos);
        void onTouchMoved(const cocos2d::Vec2& pPos);
        void onTouchEnded(const cocos2d::Vec2& pPos);

    public:

        ~TouchTracker();
        TouchTracker(cocos2d::Node* pTarget, const std::function<void(Swipe)>& pCallback);
    };
}