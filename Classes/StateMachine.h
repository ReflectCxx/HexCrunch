#pragma once

namespace hex
{
    template<class state_t, class derived_t>
    class StateMachine
    {
        state_t m_currentState = state_t::None;
        state_t m_previousState = state_t::None;

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
            if (pNextState == m_currentState) {
                return false;
            }

            const auto prevState = m_previousState;
            auto& derived = static_cast<derived_t&>(*this);

            if (derived.stateOnDeactivate())
            {
                m_previousState = m_currentState;
                if (derived.stateOnActivate(pNextState)) {
                    m_currentState = pNextState;
                    return true;
                }
            }
            m_previousState = prevState;
            return false;
        }
    };
}