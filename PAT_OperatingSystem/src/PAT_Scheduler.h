/**
 * @file PAT_Scheduler.h
 * @brief Scheduler class for managing tasks with different execution modes.
 *        It provides functions to execute tasks once or at intervals.
 *        The class is designed to be used as a singleton and is accessible via the `Class_Scheduler` variable.
 *        Tasks are identified by a string key, with execution times stored in a map.
 *        The key is set using the `operator[]` function and can be up to 30 characters long.
 *        The class also provides a `reset` function to reset the last execution time.
 *
 * Types:
 * - key: String
 * - func: std::function<void()>
 * - interval: unsigned long
 * - timeUpdated: unsigned long
 * - return: ExecuteOutput
 */

#ifndef __PAT_Scheduler_H
#define __PAT_Scheduler_H

#include <Arduino.h>
#include <map>
#include <functional>
#include <string>

enum class ExecuteMode : uint8_t
{
    Null = 0,
    Once,
    Interval
};

enum class ExecuteOutput : uint8_t
{
    ExecuteError = 0,
    ExecuteRunning,
    ExecuteDone,
    ExecuteWaiting
};

//------------------------------------------------------
class Class_Scheduler
{
    std::map<String, bool> runOnceFlags;
    std::map<String, unsigned long> lastExecutionTimes;
    String key;

public:
    //---------------------------------------------------------
    ExecuteOutput Once(const std::function<void()> &func)
    {
        if (!runOnceFlags[key])
        {
            runOnceFlags[key] = true;
            func();
            return ExecuteOutput::ExecuteRunning;
        }
        return ExecuteOutput::ExecuteDone;
    }
    //---------------------------------------------------------
    ExecuteOutput Interval(unsigned long interval, unsigned long timeUpdated, const std::function<void()> &func)
    {
        unsigned long currentTime = millis();
        unsigned long lastTime = lastExecutionTimes[key];
        unsigned long timeX = timeUpdated;

        if (currentTime >= (lastTime + interval) || timeX > lastTime)
        {
            lastExecutionTimes[key] = currentTime;
            func();
            return ExecuteOutput::ExecuteRunning;
        }
        return ExecuteOutput::ExecuteWaiting;
    }
    //---------------------------------------------------------
    ExecuteOutput Interval(unsigned long intervalTime, const std::function<void()> &func)
    {
        return Interval(intervalTime, 0, func);
    }
    //---------------------------------------------------------
    Class_Scheduler() : key("") {}
    ~Class_Scheduler() {}
    //---------------------------------------------------------
    Class_Scheduler &operator[](const String &key)
    {
        this->key = key;
        return *this;
    }

    //---------------------------------------------------------
    void reset()
    {
        lastExecutionTimes[key] = 0;
    }
    //---------------------------------------------------------
};

#endif // PAT_Scheduler


