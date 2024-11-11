#include "PAT_StateMachine.h"
#include "PAT_Debug.h"

StateMachine::StateMachine() : errorHandler(nullptr), enterHandler(nullptr), exitHandler(nullptr),
                               suspended(false), force_suspend(false), logon(false),
                               Class_Log(COLOR_LIGHT_BLUE, TEXT_BOLD, "[sm]:")
{
    for (int i = 0; i < stateQueue * 2 + 1; i++)
    {
        Q[i] = "";
    }
    StartState("setIdleState", []() {});
}
void StateMachine::StartState(const String &stateName, StateFunction stateFunc)
{
    states[stateName] = stateFunc;
    if (Q[stateQueue] == "")
    {
        Q[stateQueue] = stateName; // Set the initial state
    }
    else
    {
        setNextState(stateName, 0);
    }
}
void StateMachine::StartState(const String &stateName)
{

    if (Q[stateQueue] == "")
    {
        Q[stateQueue] = stateName; // Set the initial state
    }
    else
    {
        setNextState(stateName, 0);
    }
}
void StateMachine::addState(const String &stateName, StateFunction stateFunc)
{
    states[stateName] = stateFunc; // Add or replace state function
}

void StateMachine::onEnterState(eventFunction handler)
{
    enterHandler = handler;
}

void StateMachine::onExitState(eventFunction handler)
{
    exitHandler = handler;
}

void StateMachine::setErrorHandler(eventFunction handler)
{
    errorHandler = handler;
}
void StateMachine::logOff(void)
{
    logon = false;
    enterHandler = nullptr;
    exitHandler = nullptr;
    errorHandler = nullptr;
}

void StateMachine::logOn(String stateMachineName)
{
    init(COLOR_LIGHT_BLUE, TEXT_BOLD, "[sm]:[%s]: ", stateMachineName.c_str());
    logon = true;

    onEnterState([&](const String &stateName)
                 {
                     //------------------------------
                     log(COLOR_LIGHT_GREEN, TEXT_NORMAL, "Entering state: %s\n", stateName.c_str());
                     //------------------------------
                 });
    onExitState([&](const String &stateName)
                {
                    //------------------------------
                    log(COLOR_TEAL, TEXT_NORMAL, "Exiting state: %s\n", stateName.c_str());
                    //------------------------------
                });
    setErrorHandler([&](const String &errorMessage)
                    {
                        //------------------------------
                        log(COLOR_RED, TEXT_NORMAL, "Error state not found:  %s\n", errorMessage.c_str());
                        //------------------------------
                    });
}
void StateMachine::update()
{
    if (!force_suspend)
    {
        //----------------------------------------
        // Check if a state transition is needed
        if (Q[stateQueue + 1] != "")
        {
            if (millis() >= QDelay[stateQueue + 1])
            {
                executeTransition();
            }
        }
        else if (states.count(Q[stateQueue]) > 0) // Execute the current state's function
        {
            states[Q[stateQueue]]();
        }
        else if (errorHandler)
        {
            handleError("State not found: " + Q[stateQueue]);
        }
        //----------------------------------------
    }
    else
    {
        //----------------------------------------
        if (!suspended && logon)
        {
            log(COLOR_ORANGE, TEXT_NORMAL, "suspended %s\n", Q[stateQueue].c_str());
        }
        suspended = true;
        //----------------------------------------
    }
}

bool StateMachine::forcedSuspend(unsigned long timeout)
{
    unsigned long startTime = millis();
    force_suspend = true;
    while ((!suspended) && (millis() - startTime < timeout))
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    return suspended;
}
bool StateMachine::isSuspended(void)
{
    return suspended;
}
void StateMachine::suspend(void)
{
    force_suspend = true;
}
void StateMachine::resume(void)
{
    if (force_suspend && logon)
    {
        log(COLOR_ORANGE, TEXT_NORMAL, "resume %s\n", Q[stateQueue].c_str());
    }
    force_suspend = false;
}

void StateMachine::setIdleState(int execute_idle_state_after_delay)
{
    setNextState("setIdleState", execute_idle_state_after_delay);
}
void StateMachine::setNextState(const String &stateName, int execute_next_state_after_delay)
{
    bool exist = false;
    for (int i = 1; i < stateQueue * 2 + 1; i++)
    {
        if (Q[stateQueue + i] == "")
        {
            QDelay[stateQueue + i] = execute_next_state_after_delay + millis();
            Q[stateQueue + i] = stateName;
            exist = true;
            break;
        }
    }
    if (!exist && logon)
    {
        log(COLOR_RED, TEXT_NORMAL, "Error next state queue is full for %s\n", stateName.c_str());
    }
}
bool StateMachine::recentlyDone(const String &stateName)
{
    return ((Q[stateQueue + 1] != stateName) && (Q[stateQueue] != stateName) && (Q[stateQueue - 1] == stateName));
}
String StateMachine::getState(int state)
{
    return getQ(state);
}

    String StateMachine::getQ(int state)
    {
        int Qstate = state + stateQueue;
        if (Qstate >= 0 && Qstate < stateQueue * 2 + 1)
        {
            return Q[Qstate];
        }
        if (logon)
        {
            log(COLOR_RED, TEXT_NORMAL, "Error input of getQ is not valid\n");
        }
        return "";
    }
    void StateMachine::printQ(void)
    {
        String str = "";
        for (int i = 0; i < stateQueue * 2; i++)
        {
            if (Q[i] != "")
            {
                str += "Q[" + String(i - stateQueue) + "]= " + Q[i] + " ,";
            }
        }

        if (Q[stateQueue * 2] != "")
        {
            str += "Q[" + String(stateQueue) + "]= " + Q[stateQueue * 2];
        }
        log(COLOR_LIGHT_MAGENTA, TEXT_NORMAL, "%s\n", str.c_str());
    }
    void StateMachine::cleanState(void)
    {
        for (int i = 0; i < stateQueue * 2 + 1; i++)
        {
            Q[i] = "";
        }
    }
    void StateMachine::executeTransition()
    {
        // Call exit handler for the current state
        if (exitHandler)
        {
            exitHandler(Q[stateQueue]);
        }
        // Perform state transition (shift states down the line)
        for (int i = 0; i < stateQueue * 2; i++)
        {
            Q[i] = Q[i + 1];
            QDelay[i] = QDelay[i + 1];
        }
        Q[stateQueue * 2] = "";
        QDelay[stateQueue * 2] = 0;

        // Call enter handler for the new state
        if (enterHandler)
        {
            enterHandler(Q[stateQueue]);
        }
    }
    void StateMachine::handleError(const String &errorMessage)
    {
        if (errorHandler)
        {
            errorHandler(errorMessage);
        }
    }