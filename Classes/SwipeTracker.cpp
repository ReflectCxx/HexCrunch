#include "SwipeTracker.h"

USING_NS_CC;

namespace {

    constexpr float MIN_SWIPE_DISTANCE = 100.f;
}

namespace hex
{
    SwipeTracker::SwipeTracker(Node* pTarget, Callback pCallback)
        : m_isSwiping(false)
        , m_callback(std::move(pCallback))
        , m_target(pTarget)
    {
        init();
    }


    SwipeTracker::~SwipeTracker()
    {
        if (m_listener && m_target) {
            m_target->getEventDispatcher()->removeEventListener(m_listener);
        }
    }


    void SwipeTracker::init()
    {
        m_listener = cocos2d::EventListenerTouchOneByOne::create();
        m_listener->onTouchBegan = [this](cocos2d::Touch* pTouch, cocos2d::Event*)
        {
            m_startPos = pTouch->getLocation();
            m_isSwiping = false;
            return true;
        };

        m_listener->onTouchMoved = [this](cocos2d::Touch* pTouch, cocos2d::Event*)
        {
            const auto currentPos = pTouch->getLocation();
            const auto delta = currentPos - m_startPos;

            // Determine the dominant direction of movement.
            if (std::abs(delta.x) > std::abs(delta.y))
            {
                if (std::abs(delta.x) < MIN_SWIPE_DISTANCE) {
                    return;
                }

                m_callback(delta.x > 0 ? Swipe::kRight : Swipe::kLeft);
            }
            else
            {
                if (std::abs(delta.y) < MIN_SWIPE_DISTANCE) {
                    return;
                }

                m_callback(delta.y > 0 ? Swipe::kUp : Swipe::kDown);
            }
            m_isSwiping = true;
            m_startPos = currentPos;
        };

        m_listener->onTouchEnded = [this](cocos2d::Touch*, cocos2d::Event*)
        {
            if (!m_isSwiping) {
                m_callback(Swipe::kSingleTap);
            }
        };

        const auto dispatcher = m_target->getEventDispatcher();
        dispatcher->addEventListenerWithSceneGraphPriority(m_listener, m_target);
    }
}