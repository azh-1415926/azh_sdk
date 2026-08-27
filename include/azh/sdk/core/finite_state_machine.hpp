#pragma once

#include <functional>
#include <vector>

namespace azh::sdk::core
{

/**
 * @brief A generic finite state machine (FSM) implementation.
 *
 * This class allows defining states, events, and transitions between states.
 * Each transition can optionally execute an action when triggered.
 *
 * @tparam state_enum The enumeration type used for states.
 * @tparam event_enum The enumeration type used for events.
 */
template<typename state_enum, typename event_enum>
class finite_state_machine
{
public:
    /**
     * @brief Represents a single transition rule.
     */
    struct transition
    {
        state_enum current_state;     ///< Source state of the transition.
        event_enum event;             ///< Event that triggers the transition.
        state_enum next_state;        ///< Destination state after the transition.
        std::function<void()> action; ///< Optional action to execute on transition.
    };

    /**
     * @brief Default constructor. Initializes states to default values.
     */
    finite_state_machine()
        : m_current_state()
        , m_initial_state()
    {}

    /**
     * @brief Constructor with an initial state.
     * @param initial_state The initial state of the machine.
     */
    explicit finite_state_machine(state_enum initial_state)
        : m_current_state(initial_state)
        , m_initial_state(initial_state)
    {}

    /**
     * @brief Sets the initial state and resets the current state to it.
     * @param initial_state The new initial state.
     */
    void setInitialState(state_enum initial_state)
    {
        m_initial_state = initial_state;
        m_current_state = initial_state;
    }

    /**
     * @brief Adds a transition rule to the FSM.
     * @param current_state Source state.
     * @param event Triggering event.
     * @param next_state Destination state.
     * @param action Optional action to execute when the transition occurs.
     */
    void addTransition(
        state_enum current_state,
        event_enum event,
        state_enum next_state,
        std::function<void()> action = nullptr)
    {
        transition m_transition;
        m_transition.current_state = current_state;
        m_transition.event = event;
        m_transition.next_state = next_state;
        m_transition.action = action;
        m_transitions.push_back(m_transition);
    }

    /**
     * @brief Triggers an event, causing a transition if a matching rule exists.
     * @param event The event to trigger.
     * @return true if a transition was performed, false otherwise.
     */
    bool triggerEvent(event_enum event)
    {
        for (auto m_it = m_transitions.begin(); m_it != m_transitions.end(); ++m_it) {
            if (m_it->current_state == m_current_state && m_it->event == event) {
                m_current_state = m_it->next_state;
                if (m_it->action) {
                    m_it->action();
                }
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Gets the current state of the FSM.
     * @return The current state.
     */
    state_enum getCurrentState() const { return m_current_state; }

    /**
     * @brief Resets the FSM to its initial state.
     */
    void reset() { m_current_state = m_initial_state; }

private:
    state_enum m_current_state;            ///< Current state of the machine.
    state_enum m_initial_state;            ///< Initial state (used for reset).
    std::vector<transition> m_transitions; ///< Collection of transition rules.
};
} // namespace azh::sdk::core