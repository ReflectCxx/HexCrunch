#include "Constants.h"
#include "TouchTracker.h"

USING_NS_CC;

namespace {
    constexpr float MIN_SWIPE_DISTANCE = hex::SCALE * 100.f;
    constexpr float MIN_TAP_DRIFT_OFFSET = hex::SCALE * 15.f;
    constexpr float TAP_COOLDOWN_AFTER_SWIPE = 0.18f;
}

namespace hex
{
    TouchTracker::TouchTracker(Node* pTarget, const std::function<void(Swipe)>& pCallback)
        : m_isSwiping(false)
        , m_isTapCandidate(false)
        , m_callback(pCallback)
        , m_startPos(Vec2::ZERO)
        , m_target(pTarget)
        , m_listener(nullptr)
        , m_lastSwipeEndTime{} {
        init();
    }


    TouchTracker::~TouchTracker()
    {
        if (m_listener && m_target) {
            m_target->getEventDispatcher()->removeEventListener(m_listener);
        }
    }


    void TouchTracker::init()
    {
        m_listener = EventListenerTouchOneByOne::create();
        m_listener->onTouchBegan = [this](Touch* pTouch, Event*) {
            onTouchBegan(pTouch->getLocation());
            return true;
        };

        m_listener->onTouchMoved = [this](Touch* pTouch, Event*) {
            onTouchMoved(pTouch->getLocation());
        };

        m_listener->onTouchEnded = [this](Touch* pTouch, Event*) {
            onTouchEnded(pTouch->getLocation());
        };
        const auto dispatcher = m_target->getEventDispatcher();
        dispatcher->addEventListenerWithSceneGraphPriority(m_listener, m_target);
    }
}



namespace hex
{
    void TouchTracker::onTouchBegan(const Vec2& pPos)
    {
        m_startPos = pPos;
        m_isSwiping = false;
        m_isTapCandidate = true;
    }


    void TouchTracker::onTouchMoved(const Vec2& pPos)
    {
        constexpr auto DS = (MIN_TAP_DRIFT_OFFSET * MIN_TAP_DRIFT_OFFSET);
        if (m_isTapCandidate && pPos.distanceSquared(m_startPos) > DS) {
            m_isTapCandidate = false;
        }

        const auto delta = (pPos - m_startPos);
        if (std::abs(delta.x) > std::abs(delta.y)) {
            if (std::abs(delta.x) < MIN_SWIPE_DISTANCE) {
                return;
            }
            m_callback(delta.x > 0 ? Swipe::Right : Swipe::Left);
        }
        else {
            if (std::abs(delta.y) < MIN_SWIPE_DISTANCE) {
                return;
            }
            m_callback(delta.y > 0 ? Swipe::Up : Swipe::Down);
        }
        m_isSwiping = true;
        m_startPos = pPos;
    }


    void TouchTracker::onTouchEnded(const Vec2& pPos)
    {
        const auto currentTime = std::chrono::steady_clock::now();

        if (m_isSwiping) {
            m_lastSwipeEndTime = currentTime;
            return;
        }

        if (m_lastSwipeEndTime.time_since_epoch().count() != 0)
        {
            const auto dt = (currentTime - m_lastSwipeEndTime);
            const auto interval = std::chrono::duration<float>(dt).count();
            if (interval < TAP_COOLDOWN_AFTER_SWIPE) {
                return;
            }
        }

        if (m_isTapCandidate) {
            m_callback(Swipe::SingleTap);
        }
    }
}