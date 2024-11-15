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
class JsonStorage : public Observer, public Subject, public Class_Log
{
private:
  bool logon;
  std::vector<Observer *>
      observers; // List of observers
  std::mutex update_mutex;
  const char *filePath, *nameFile, *filePath_backup, *defaultJsonString;
  const int json_size;
  std::function<void(JsonVariant)> updateCallback;
  DynamicJsonDocument *jsonDoc;
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
  JsonStorage(const char *filePath, const char *defaultJsonString, size_t json_size)
      : filePath(filePath),
        defaultJsonString(defaultJsonString),
        jsonDoc(nullptr),
        json_size(json_size),
        mutex(xSemaphoreCreateMutex()),
        updateCallback(nullptr),
        openedStatus(LOADED_ERROR),
        logon(false),
        Class_Log(COLOR_MAGENTA, TEXT_BOLD, "[js]: ")
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
    Class_Log::init(COLOR_LIGHT_BLUE, TEXT_BOLD, "[sm]:[%s]: ", name);
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
            log(COLOR_GREEN, TEXT_NORMAL, "updateCallback starting for: %s\n", nameFile);

        updateCallback(json);

        if (logon)
            log(COLOR_GREEN, TEXT_NORMAL, "updateCallback finished for: %s\n", nameFile);

        if (wasOpenedError)
        {
            close();
        }
    }
}
  void setUpdateCallback(std::function<void(JsonVariant)> callback)
  {
    updateCallback = callback;
  }

  void notifyObservers() override
  {
    for (auto observer : observers)
    {

      observer->update(json().as<JsonVariant>()); // Notify each observer
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