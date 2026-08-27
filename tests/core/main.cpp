#include <azh/sdk/core/finite_state_machine.hpp>
#include <azh/sdk/type/string.hpp>
#include <azh/sdk/utils/logger.hpp>


using namespace azh::sdk::core;

// Define state and event enums (lowercase with underscores as needed)
enum class door_state { closed, open };

enum class door_event { open_event, close_event };

void test_fsm()
{
    // Create finite state machine with initial state "closed"
    finite_state_machine<door_state, door_event> m_fsm(door_state::closed);

    // Add transition: closed + open_event -> open
    m_fsm.addTransition(door_state::closed, door_event::open_event, door_state::open, []() {
        aDebug() << "Door is opening...";
    });

    // Add transition: open + close_event -> closed
    m_fsm.addTransition(door_state::open, door_event::close_event, door_state::closed, []() {
        aDebug() << "Door is closing...";
    });

    // Output initial state
    if (m_fsm.getCurrentState() == door_state::closed) {
        aDebug() << "Initial state: closed";
    }

    // Trigger open event
    m_fsm.triggerEvent(door_event::open_event);
    if (m_fsm.getCurrentState() == door_state::open) {
        aDebug() << "Current state: open";
    }

    // Trigger close event
    m_fsm.triggerEvent(door_event::close_event);
    if (m_fsm.getCurrentState() == door_state::closed) {
        aDebug() << "Current state: closed";
    }
}

int main()
{
    test_fsm();
    
    return 0;
}