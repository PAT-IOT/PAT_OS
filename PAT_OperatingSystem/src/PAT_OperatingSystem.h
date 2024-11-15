#ifndef PAT_OperatingSystem__H
#define PAT_OperatingSystem__H
#include <Arduino.h>
#include <map>
#include <functional>
#include <string>
#include <variant>
#include <typeindex>
#include <stdexcept>
#include <memory>
//--------------------------
#include "PAT_ESP.h"
#include "PAT_QueueManager.h"
#include "PAT_StateMachine.h"
#include "PAT_EventVariable.h"
#include "PAT_VarManager.h"
#include "esp_task_wdt.h"
#include "PAT_ObserverSubject.h"
#include "PAT_JsonStorage.h"
#include "PAT_Scheduler.h"
#include "PAT_Debug.h"
//---------------------------

#define UNIQUE_KEY (String(__FILE__) + "_" + String(__LINE__))
//------------------------------------------------------

// extern const char *db_Configuration_json;
class operatingSystem : public Class_Log
{

    queueManager queue;
    std::map<String, bool> runOnceFlags;
    std::map<String, unsigned long> lastExecutionTimes;
    using StateFunction = std::function<void()>;
    // std::map<std::string, std::unique_ptr<VarBase>> var;

public:
    static Class_ESP esp;
    std::map<String, StateMachine> sm;
    // template <typename T>
    Event<void *> *ptr;
    static JsonStorage db;
    Class_Scheduler execute;
    //---------------------------------------------------------

    operatingSystem() : Class_Log(COLOR_MAGENTA, TEXT_BOLD, "[os]: ")
    {
        // Class_Log::init(COLOR_MAGENTA, TEXT_REVERSE, "[os]: ");
    }

    ~operatingSystem()
    {
    }
    void init();
    void initVerbose();
    String randomKey(int tokenLength = 20);
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // PAT_OperatingSystem
//------------------------------------------------------
extern operatingSystem os;
// Define a macro for executing functions at intervals
