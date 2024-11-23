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
  //----------------------------------------------------------------------------------------------------------------------------
private:
  // T *injectedObject;
  std::vector<Observer *> observers; // List of observers
  std::mutex update_mutex;
  const char *filePath, *nameFile, *filePath_backup, *defaultJsonString;
  const int json_size;
  std::function<void(JsonVariant)> updateCallback;
  std::function<void()> prepareForNotification;
  DynamicJsonDocument *jsonDoc;
  fileStatus openedStatus;
  const char *getBackupFilePath(const char *filePath) const;
  bool loadFromFile(const char *path, int maxRetries = 3);
  const char *getNameFile(const char *filePath) const;

  fileStatus initDefaultJson();
  bool saveToFile(const char *path, const char *data);
  fileStatus load(int maxRetries = 3);

  //----------------------------------------------------------------------------------------------------------------------------
protected:
  bool init();
  //----------------------------------------------------------------------------------------------------------------------------
public:
  SemaphoreHandle_t mutex;

  JsonStorage(const char *filePath, const char *defaultJsonString, size_t json_size) //,  T* injectedObj = nullptr
      : filePath(filePath),
        defaultJsonString(defaultJsonString),
        jsonDoc(nullptr),
        json_size(json_size),
        mutex(xSemaphoreCreateMutex()),
        updateCallback(nullptr),
        prepareForNotification(nullptr),
        openedStatus(LOADED_ERROR),
        Class_Log(COLOR_MAGENTA, TEXT_BOLD, "[js]:")
  {
    filePath_backup = getBackupFilePath(filePath);
    nameFile = getNameFile(filePath);
  }
  //-----------------------------------

  ~JsonStorage();
  //-----------------------------------
  fileStatus openStatus(void)
  {
    return openedStatus;
  }
  //-----------------------------------
  String getName(const char *filePath) const;
  // JsonStorage(const char *filePath, const char *defaultJsonString, size_t json_size = 1024);
  //-----------------------------------
  void logOn(String name = "") override
  {
    if (name == "")
      name = String(nameFile);

    Class_Log::init(COLOR_MAGENTA, TEXT_BOLD, "[%s]:", name.c_str());
    setLogOn();
  }
  //-----------------------------------

  //-----------------------------------
  fileStatus open();
  bool save();
  bool close();
  int max(const char *key);
  DynamicJsonDocument &json();
  JsonVariant operator[](const char *key);
  void print();
  //------------------------------------------------------------------------------------------------------------------------------

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
    if (!updateCallback)
      return;

    if (openedStatus == LOADED_ERROR && open() == LOADED_ERROR)
      return;

    log(COLOR_LIGHT_BLUE, TEXT_NORMAL, "updateCallback starting\n");

      updateCallback(json);

      log(COLOR_BLUE, TEXT_NORMAL, "updateCallback finished\n");
  }

  //-----------------------------------
  void setUpdateCallback(std::function<void(JsonVariant)> callback)
  {
    updateCallback = callback;
  }
  //-----------------------------------
  void setPrepareForNotifyCallback(std::function<void()> callback)
  {
    prepareForNotification = callback;
  }
  //-----------------------------------
  void notifyObservers() override
  {
    if (openedStatus == LOADED_ERROR && open() == LOADED_ERROR)
      return;

    if (prepareForNotification)
    {

        log(COLOR_BLUE, TEXT_NORMAL, "setPrepareForNotifyCallback is starting\n");
      prepareForNotification();
    }

    for (auto &observer : observers)
    {
      observer->update(json().as<JsonVariant>());
    }
  }
  //-----------------------------------
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