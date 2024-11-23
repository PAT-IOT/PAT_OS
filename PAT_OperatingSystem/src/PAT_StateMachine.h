#ifndef PAT_STATE_MACHINE_H
#define PAT_STATE_MACHINE_H

#include <Arduino.h>
#include <map>
// #include <queue>
#include <functional>
#include <string>
#include <ArduinoJson.h>
#include "PAT_Debug.h"
#define stateQueue 10

class StateMachine : public Class_Log
{
private:
    using StateFunction = std::function<void()>;
    using eventFunction = std::function<void(const String &)>;
    using statesFunction = std::map<String, StateFunction>;

    statesFunction states;
    eventFunction enterHandler, exitHandler, errorHandler;

    uint32_t QDelay[stateQueue * 2 + 1];
    String Q[stateQueue * 2 + 1];

    bool suspended, force_suspend;

    // Execute state transitions if needed
    void executeTransition();
    void handleError(const String &errorMessage);
    //-----------------------------------------------------------------------------------
public:
    StateMachine();
    void logOn(String stateMachineName) override;
    void logOff(void) override;
    // Initialize the state machine with an initial state
    void StartState(const String &stateName, StateFunction stateFunc);
    void StartState(const String &stateName);

    // Dynamically add a new state
    void addState(const String &stateName, StateFunction stateFunc);

    // Register transition handlers for entering and exiting states
    void onEnterState(eventFunction handler);
    void onExitState(eventFunction handler);

    // Register an error handler
    void setErrorHandler(eventFunction handler);

    // Update method to execute the current state logic
    void update();
    bool recentlyDone(const String &stateName);
    bool forcedSuspend(unsigned long timeout = 30000);
    bool isSuspended(void);
    void suspend(void);
    void resume(void); // Set the next state to transition to
    void setNextState(const String &stateName, int execute_next_state_after_delay = 0);
    void setIdleState(int execute_idle_state_after_delay = 0);
    void cleanState(void);
    String getQ(int state);
    String getState(int state);
    void printQ(void);
};

#endif // PAT_STATE_MACHINE_H