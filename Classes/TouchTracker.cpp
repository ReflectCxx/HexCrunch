#include "Constants.h"
#include "TouchTracker.h"

USING_NS_CC;

namespace {
    constexpr float MIN_SWIPE_DISTANCE = hex::SCALE * 100.f;
    constexpr float MIN_TAP_DRIFT_OFFSET = hex::SCALE * 15.f;
}

namespace hex
{
    TouchTracker::TouchTracker(Node* pTarget, const std::function<void(Swipe)>& pCallback)
        : m_isSwiping(false)
        , m_isTapCandidate(false)
        , m_callback(pCallback)
        , m_startPos(Vec2::ZERO)
        , m_target(pTarget)
        , m_listener(nullptr) {
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
    void TouchTracker::onTouchEnded(const Vec2& pPos)
    {
        const auto drift = pPos.distance(m_startPos);
        CCLOG("Tap loc offset : %f", drift);

        if (!m_isSwiping && m_isTapCandidate) {
            m_callback(Swipe::kSingleTap);
        }
    }

    void TouchTracker::onTouchBegan(const Vec2& pPos)
    {
        m_startPos = pPos;
        m_isSwiping = false;
        m_isTapCandidate = true;
    }

    void TouchTracker::onTouchMoved(const Vec2& pPos)
    {
        if (m_isTapCandidate && pPos.distance(m_startPos) > MIN_TAP_DRIFT_OFFSET) {
            m_isTapCandidate = false;
        }

        const auto delta = (pPos - m_startPos);
        if (std::abs(delta.x) > std::abs(delta.y)) {
            if (std::abs(delta.x) < MIN_SWIPE_DISTANCE) {
                return;
            }
            m_callback(delta.x > 0 ? Swipe::kRight : Swipe::kLeft);
        }
        else {
            if (std::abs(delta.y) < MIN_SWIPE_DISTANCE) {
                return;
            }
            m_callback(delta.y > 0 ? Swipe::kUp : Swipe::kDown);
        }
        m_isSwiping = true;
        m_startPos = pPos;
    }
}