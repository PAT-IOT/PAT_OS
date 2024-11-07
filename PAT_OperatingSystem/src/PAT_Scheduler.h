#ifndef __PAT_Scheduler_H
#define __PAT_Scheduler_H
#include <Arduino.h>
#include <map>
#include <functional>
#include <string>
// Remove duplicate includes
#include "PAT_ESP.h"
#include "esp_task_wdt.h"

enum executeMode
{
    Null = 0,
    ONCE,
    INTERVAL
};

enum executeOutput
{
    EXECUTE_ERROR = 0,
    EXECUTE_RUNNING,
    EXECUTE_DONE,
    EXECUTE_WAITING
};

//------------------------------------------------------
class Class_Scheduler
{
    std::map<String, bool> runOnceFlags;
    std::map<String, unsigned long> lastExecutionTimes;
    using function = std::function<void()>;
    String key;
    Class_Scheduler *ptrScheduler;

public:
    //---------------------------------------------------------
    executeOutput Once(function func)
    {
        // Serial.println(this->key);
        if (!runOnceFlags[this->key]) // Cleaner check for `false`
        {
            runOnceFlags[this->key] = true;
            func();
            return EXECUTE_RUNNING;
        }
        return EXECUTE_DONE;
    }
    //---------------------------------------------------------
    executeOutput Interval(unsigned long interval, unsigned long timeUpdated, function func)
    {
        unsigned long currentTime = millis();
        unsigned long lastTime = lastExecutionTimes[this->key];
        unsigned long timeX = timeUpdated;

        if (currentTime >= (lastTime + interval) || timeX > lastTime)
        {
            lastExecutionTimes[this->key] = currentTime;
            func();
            return EXECUTE_RUNNING;
        }
        return EXECUTE_WAITING;
    }
    //---------------------------------------------------------
    executeOutput Interval(unsigned long interval_time, function func)
    {
        // Serial.println(this->key);
        return this->Interval(interval_time, 0, func);
    }
    //---------------------------------------------------------

    //---------------------------------------------------------
    Class_Scheduler() : key("")
    {
        this->key.reserve(30);
    }
    ~Class_Scheduler() {}
    //---------------------------------------------------------
    Class_Scheduler &operator[](const String &_key) // Return by reference
    {
        if (key.length() > 30)
        {
            this->key.reserve(key.length() + 1);
        }
        this->key = _key; // Set the key

        // Serial.println("Key set: " + this->key); // Print to confirm
        return *this; // Return reference to the current object
    }

    //---------------------------------------------------------
};
#endif // PAT_Scheduler
