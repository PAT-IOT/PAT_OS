#include "PAT_StateMachine.h"
#include "PAT_Debug.h"

StateMachine::StateMachine() : currentState(""), errorHandler(nullptr),
                               Q_1_State(""), Q_2_State(""), Q_3_State(""),
                               Q_4_State(""), Q_5_State(""), Q_6_State(""), suspended(false), stateMachineName(""), force_suspend(false), logon(false), Class_Log(COLOR_LIGHT_BLUE, TEXT_BOLD, "[sm]:")
{
    for (int i = 0; i < _STATE_BUFFER; i++)
    {
        nextState[i] = "";
    }
}
// Destructor: Delete all created queues
// StateMachine::~StateMachine()
// {
//     for (auto &queuePair : sendQueues)
//     {
//         if (queuePair.second != nullptr)
//         {
//             vQueueDelete(queuePair.second);
//         }
//     }
//     for (auto &queuePair : receiveQueues)
//     {
//         if (queuePair.second != nullptr)
//         {
//             vQueueDelete(queuePair.second);
//         }
//     }
// }
// Create a queue if it doesn't exist for the specified key

void StateMachine::StartState(const String &stateName, StateFunction stateFunc)
{
    states[stateName] = stateFunc;
    if (currentState == "")
    {
        currentState = stateName; // Set the initial state
    }
}
void StateMachine::StartState(const String &stateName)
{

    if (currentState == "")
    {
        currentState = stateName; // Set the initial state
    }
}
void StateMachine::addState(const String &stateName, StateFunction stateFunc)
{
    states[stateName] = stateFunc; // Add or replace state function
}

void StateMachine::onEnterState(StateTransitionHandler handler)
{
    enterHandler = handler;
}

void StateMachine::onExitState(StateTransitionHandler handler)
{
    exitHandler = handler;
}

void StateMachine::setErrorHandler(ErrorHandler handler)
{
    errorHandler = handler;
}
void StateMachine::logOn(String statemachinename)
{
    logon = true;
    stateMachineName = statemachinename;
    onEnterState([&](const String &stateName)
                 {
                     //------------------------------
                     log(COLOR_LIGHT_GREEN, TEXT_NORMAL, "[%s]: Entering state: %s\n", stateMachineName.c_str(), stateName.c_str());
                     //------------------------------
                 });
    onExitState([&](const String &stateName)
                {
                    //------------------------------
                    log(COLOR_TEAL, TEXT_NORMAL, "[%s]: Exiting state: %s\n", stateMachineName.c_str(), stateName.c_str());
                    //------------------------------
                });
    setErrorHandler([&](const String &errorMessage)
                    {
                        //------------------------------
                        log(COLOR_RED, TEXT_NORMAL, "[%s]: Error state not found:  %s\n", stateMachineName.c_str(), errorMessage.c_str());
                        //------------------------------
                    });
}
void StateMachine::update()
{
    if (!force_suspend)
    {
        // Check if a state transition is needed
        if (nextState[0] != "")
        {
            if (millis() >= executeNextStateWithDelay[0])
            {
                executeTransition();
            }
        }

        // Execute the current state's function
        if (states.count(currentState) > 0)
        {
            states[currentState]();
        }
        else if (errorHandler)
        {
            handleError("State not found: " + currentState);
        }
    }
    else
    {
        if (!suspended && logon)
        {
            log(COLOR_ORANGE, TEXT_NORMAL, "[%s]: suspended %s\n", stateMachineName.c_str(), currentState.c_str());
        }
        suspended = true;
    }
}

bool StateMachine::forcedSuspend(unsigned long timeout)
{
    unsigned long startTime = millis();
    force_suspend = true;
    while ((!suspended) && (millis() - startTime < timeout))
    {
        delay(100);
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
        log(COLOR_ORANGE, TEXT_NORMAL, "[%s]: resume %s\n", stateMachineName.c_str(), currentState.c_str());
    }
    force_suspend = false;
}

void StateMachine::setNextState(const String &stateName, int execute_Next_State_With_Delay)
{
    bool exist = false;
    for (int i = 0; i < _STATE_BUFFER; i++)
    {
        if (nextState[i] == "")
        {
            executeNextStateWithDelay[i] = execute_Next_State_With_Delay + millis();
            nextState[i] = stateName;
            exist = true;
            break;
        }
    }
    if (!exist)
    {
        log(COLOR_RED, TEXT_NORMAL, "[%s]: Error next state queue is full for %s\n", stateMachineName.c_str(), stateName.c_str());
    }
}
bool StateMachine::recentlyDone(const String &stateName)
{
    return ((nextState[0] != stateName) && (currentState != stateName) && (Q_1_State == stateName));
}
String StateMachine::getState(stateCode state)
{
    switch (state)
    {
    case current:
        return currentState;
    case next:
        return nextState[0];
    case Q_1:
        return Q_1_State;
    case Q_2:
        return Q_2_State;
    case Q_3:
        return Q_3_State;
    case Q_4:
        return Q_4_State;
    case Q_5:
        return Q_5_State;
    case Q_6:
        return Q_6_State;
    default:
        return "";
    }
}
void StateMachine::cleanState(void)
{
    Q_6_State = "";
    Q_5_State = "";
    Q_4_State = "";
    Q_3_State = "";
    Q_2_State = "";
    Q_1_State = "";
    currentState = "";
    ;
    nextState[0] = "";
}
void StateMachine::executeTransition()
{
    // Call exit handler for the current state
    if (exitHandler)
    {
        exitHandler(currentState);
    }
    // Perform state transition (shift states down the line)
    Q_6_State = Q_5_State;
    Q_5_State = Q_4_State;
    Q_4_State = Q_3_State;
    Q_3_State = Q_2_State;
    Q_2_State = Q_1_State;
    Q_1_State = currentState;
    // Update current state
    currentState = nextState[0];
    for (int i = 0; i < _STATE_BUFFER - 2; i++)
    {
        nextState[i] = nextState[i + 1];
    }
    nextState[_STATE_BUFFER - 1] = "";

    // Call enter handler for the new state
    if (enterHandler)
    {
        enterHandler(currentState);
    }
}

void StateMachine::handleError(const String &errorMessage)
{
    if (errorHandler)
    {
        errorHandler(errorMessage);
    }
}
//============================================================================================================================================
// // Function to send a report for a given state
// void StateMachine::sendReport(const String &stateName, const JsonObject &report)
// {
//     // Allocate a new DynamicJsonDocument for this state
//     stateReports[stateName] = DynamicJsonDocument(1024);
//     // Copy the contents of the report to the new document
//     stateReports[stateName].set(report);
// }

// //============================================================================================================================================
// // Function to wait for a report for a given state, with a timeout
// bool StateMachine::waitForReport(const String &stateName, JsonObject &report, int timeoutMs)
// {
//     int timeElapsed = 0;
//     const int pollingInterval = 100; // Poll every 100ms
//     // Continue polling until the report is available or timeout occurs
//     while (timeElapsed < timeoutMs)
//     {
//         // Check if the report for the given state is available
//         auto it = stateReports.find(stateName);
//         if (it != stateReports.end())
//         {
//             // Get the JsonObject from the stored DynamicJsonDocument
//             report = it->second.as<JsonObject>();

//             // Optionally, remove the report after retrieval
//             stateReports.erase(it);

//             return true;
//         }
//         // Delay to avoid busy-waiting (FreeRTOS delay)
//         vTaskDelay(pdMS_TO_TICKS(pollingInterval));
//         // Increment the elapsed time
//         timeElapsed += pollingInterval;
//     }
//     // If no report was received within the timeout, return an empty report
//     DynamicJsonDocument errorReport(256);  // Create a document for the error case
//     report = errorReport.to<JsonObject>(); // Return an empty object
//     return false;                          // Indicate timeout
// }