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
#include "PAT_JsonStorage.h"
#include "PAT_Scheduler.h"
#include "PAT_Debug.h"
//---------------------------







// #include <ArduinoJson.h>
// #include <map>
// #include <String.h>

// class Report:public JsonStorage{
// public:
//     // Operator to allow dynamic access like report["key"]
//     DynamicJsonDocument& operator[](const String& key);
    
//     // Method to send data from the report
//     void send(const String& key);

//     // Method to store internal data
//     void store(const String& key, const DynamicJsonDocument& data);

// private:
//     std::map<String, Storage> reportSections(1024);  // Store different report sections
// };

// // Operator overloading for dynamic access
// DynamicJsonDocument& Report::operator[](const String& key) {
//     // Check if the key exists, if not create a new section
//     if (reportSections.find(key) == reportSections.end()) {
//         reportSections[key] = DynamicJsonDocument(1024);  // Adjust size as needed
//     }
//     return reportSections[key];  // Return reference to the stored DynamicJsonDocument
// }

// // Method to send data
// void Report::send(const String& key) {
//     // Check if the key exists
//     if (reportSections.find(key) != reportSections.end()) {
//         // Logic for sending data (e.g., serial, network, etc.)
//         serializeJson(reportSections[key], Serial);  // Replace Serial with actual transport
//         Serial.println();  // Newline for clarity
//     } else {
//         Serial.println("No report found for the specified key.");  // Error handling
//     }
// }

// // Method to store internal data in a specific section
// void Report::store(const String& key, const DynamicJsonDocument& data) {
//     reportSections[key] = data;
// }

















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
