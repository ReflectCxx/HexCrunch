#pragma once

namespace hex
{
    template<class state_t, class derived_t>
    class StateMachine
    {
        state_t m_currentState = state_t::kNone;
        state_t m_previousState = state_t::kNone;

    public:

        state_t getCurrentState() const {
            return m_currentState;
        }


        state_t getPreviousState() const {
            return m_previousState;
        }


        bool stateOnDeactivate() {
            return true;
        }


        bool stateOnActivate(state_t pState)
        { }


        bool switchToState(state_t pNextState)
        {
            auto& derived = static_cast<derived_t&>(*this);
            if (derived.stateOnDeactivate() && derived.stateOnActivate(pNextState))
            {
                m_previousState = m_currentState;
                m_currentState = pNextState;
                return true;
            }
            return false;
        }
    };
}