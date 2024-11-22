#ifndef PAT_JSONSTORAGE_H
#define PAT_JSONSTORAGE_H
#include <Arduino.h>
#include <SPIFFS.h>
#include <atomic>
#include <vector>
#include <mutex>
#include <ArduinoJson.h>
#include "PAT_ObserverSubject.h"
#include "PAT_Debug.h"
//============================================================================================
// Function: copy_Keys2
// Copies specified keys from a source JsonVariant to a target JsonVariant.
// Supports copying of keys pointing to primitive values, JsonArrays, and JsonObjects.
//
// Template Parameters:
// - Args: Variadic template for keys to copy.
//
// Parameters:
// - target: The JsonVariant where keys should be copied to.
// - source: The JsonVariant from which keys should be copied.
// - keys: The keys to be copied from the source to the target.
//
// Example Usage:
//
// JsonObject sourceObj, targetObj;
// sourceObj["key1"] = "value1";
// sourceObj["key2"] = 42;
// sourceObj["key3"] = true;
// sourceObj["array"] = serialized("[1, 2, 3]");
// sourceObj["object"]["nestedKey"] = "nestedValue";
//
// copy_Keys2(targetObj, sourceObj, "key1", "array", "object");
//
// Expected Output:
// targetObj will contain:
// {
//   "key1": "value1",
//   "array": [1, 2, 3],
//   "object": { "nestedKey": "nestedValue" }
// // }
// template <typename... Args>
// void copy_Keys2(JsonVariant target, JsonVariant source, Args... keys)
// {
//   // Create an array of the keys for iteration
//   const char *keyArray[] = {keys...};

//   // Iterate over the keys
//   for (const char *key : keyArray)
//   {
//     // Check if source is a JsonObject and contains the key
//     if (source.is<JsonObject>() && source.as<JsonObject>().containsKey(key))
//     {
//       // If the source value is a JsonArray, create a new array in the target
//       if (source[key].is<JsonArray>())
//       {
//         JsonArray sourceArray = source[key].as<JsonArray>();
//         JsonArray targetArray = target.createNestedArray(key);
//         for (JsonVariant item : sourceArray)
//         {
//           targetArray.add(item); // Copy each item
//         }
//       }
//       // If the source value is a JsonObject, create a nested object in the target
//       else if (source[key].is<JsonObject>())
//       {
//         JsonObject sourceObj = source[key].as<JsonObject>();
//         JsonObject targetObj = target.createNestedObject(key);
//         for (auto it = sourceObj.begin(); it != sourceObj.end(); ++it)
//         {
//           targetObj[it->key()] = it->value(); // Copy each key-value pair
//         }
//       }
//       // For primitive values, directly assign to the target
//       else
//       {
//         target[key] = source[key];
//       }
//     }
//   }
// }
//============================================================================================
template <typename... Args>
void copy_Keys(JsonVariant target, JsonVariant source, Args... keys)
{
  // Create an array of the keys for iteration
  const char *keyArray[] = {keys...}; // Create an array from the parameter pack

  // Iterate over the keys
  for (const char *key : keyArray)
  {
    // Check if source is a JsonObject and contains the key
    if (source.is<JsonObject>() && source.as<JsonObject>().containsKey(key))
    {
      // If the source value is a JsonArray, create a new array in the target
      if (source[key].is<JsonArray>())
      {
        JsonArray sourceArray = source[key].as<JsonArray>();
        JsonArray targetArray = target.createNestedArray(key); // Create array in target
        for (JsonVariant item : sourceArray)
        {
          targetArray.add(item); // Copy each item from source array to target array
        }
      }
      // If the source value is a JsonObject, create a nested object in the target
      else if (source[key].is<JsonObject>())
      {
        JsonObject sourceObj = source[key].as<JsonObject>();
        JsonObject targetObj = target.createNestedObject(key); // Create object in target
        // Use an iterator to copy keys from sourceObj to targetObj
        for (auto it = sourceObj.begin(); it != sourceObj.end(); ++it)
        {
          targetObj[it->key()] = it->value(); // Copy each key-value pair
        }
      }
      // For regular key-value pairs, copy directly
      else
      {
        target[key] = source[key]; // Copy the value from source to target
      }
    }
  }
}
//============================================================================================
#define stateQueue 10
//============================================================================================
enum fileStatus
{
  LOADED_ERROR = 0,
  LOADED_DEFAULT_FILE = 1,
  LOADED_BACKUP_FILE = 2,
  LOADED_FILE = 3
};
//============================================================================================

// template <typename T = void>
class JsonStorage : public Observer, public Subject, public Class_Log
{
private:
  // T *injectedObject;
  bool logon;
  std::vector<Observer *>
      observers; // List of observers
  std::mutex update_mutex;
  const char *filePath, *nameFile, *filePath_backup, *defaultJsonString;
  const int json_size;
  std::function<void(JsonVariant)> updateCallback;
  std::function<void()> prepareForNotification;
  DynamicJsonDocument *jsonDoc;
  StaticJsonDocument<2048> staticJsonDoc;
  fileStatus openedStatus;
  const char *getBackupFilePath(const char *filePath) const;
  bool loadFromFile(const char *path, int maxRetries = 3);
  const char *getNameFile(const char *filePath) const;

  fileStatus initDefaultJson();
  bool saveToFile(const char *path, const char *data);
  //--------------------------------------------------------------
protected:
  bool init();

public:
  SemaphoreHandle_t mutex;

  fileStatus openStatus(void)
  {
    return openedStatus;
  }
  String getName(const char *filePath) const;
  // JsonStorage(const char *filePath, const char *defaultJsonString, size_t json_size = 1024);
  //-----------------------------------
  JsonStorage(const char *filePath, const char *defaultJsonString, size_t json_size) //,  T* injectedObj = nullptr
      : filePath(filePath),
        defaultJsonString(defaultJsonString),
        jsonDoc(nullptr),
        json_size(json_size),
        mutex(xSemaphoreCreateMutex()),
        updateCallback(nullptr),
        prepareForNotification(nullptr),
        openedStatus(LOADED_ERROR),
        logon(false), // injectedObject(injectedObj),
        Class_Log(COLOR_MAGENTA, TEXT_BOLD, "[js]:")
  {
    filePath_backup = getBackupFilePath(filePath);
    nameFile = getNameFile(filePath);
  }
  void logOn(const char *name = "")
  {

    if (name == nullptr || name[0] == '\0')
    {
      name = nameFile;
    }
    Class_Log::init(COLOR_MAGENTA, TEXT_BOLD, "[%s]:", name);
    logon = true;
  }
  void logOff(void)
  {
    logon = false;
  }
  //-----------------------------------
  ~JsonStorage();
  //-----------------------------------
  fileStatus open();
  fileStatus load(int maxRetries = 3);
  bool save();
  bool close();
  int max(const char *key);
  DynamicJsonDocument &json();
  JsonVariant operator[](const char *key);
  void print();
  void addObserver(Observer *observer) override
  {
    observers.push_back(observer);
  }

  void removeObserver(Observer *observer) override
  {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
  }
  void update(JsonVariant json) override
{
    // std::lock_guard<std::mutex> lock(update_mutex);
    if (!updateCallback) return;

    bool wasOpenedError = (openedStatus == LOADED_ERROR);
    if (wasOpenedError)
    {
        open();
    }

    if (openedStatus != LOADED_ERROR)
    {
        if (logon)
          log(COLOR_LIGHT_BLUE, TEXT_NORMAL, "updateCallback starting\n");

        updateCallback(json);

        if (logon)
          log(COLOR_BLUE, TEXT_NORMAL, "updateCallback finished\n");

        if (wasOpenedError)
        {
          // close();
        }
    }
}
  void setUpdateCallback(std::function<void(JsonVariant)> callback)
  {
    updateCallback = callback;
  }
  void setPrepareForNotifyCallback(std::function<void()> callback)
  {
    prepareForNotification = callback;
  }
  void notifyObservers() override
  {
    if (prepareForNotification)
    {
      if (logon)
        log(COLOR_BLUE, TEXT_NORMAL, "setPrepareForNotifyCallback is starting\n");
      prepareForNotification();
    }

    for (auto& observer : observers)
    {
      observer->update(json().as<JsonVariant>());
    }
  }
};

#endif // PAT_JSONSTORAGE_H

// DynamicJsonDocument *operator->()
// {
//   return jsonDoc;
// }
// const JsonArray &operator[](const char *key) const;
// DynamicJsonDocument &JsonStorage::operator->()
// {
//   return &getJson();
// }

// JsonVariant& JsonStorage::operator[](const char *key)
// {
//     if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
//     {
//         xSemaphoreGive(mutex);
//         return &jsonVariant;
//     }
//     static JsonVariant emptyVariant; // Return an empty JsonVariant if mutex is not available
//     return &emptyVariant;
// }

// DynamicJsonDocument &JsonStorage::operator->()
// {
//   return &getJson();
// }