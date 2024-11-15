#ifndef _PAT_ObserverSubject_H
#define _PAT_ObserverSubject_H
#include <Arduino.h>
#include <ArduinoJson.h>
//============================================================================================
class Observer
{
public:
  virtual void update(JsonVariant json) {}; // Now uses JsonVariant
  virtual ~Observer() {}
};
//============================================================================================
// Observer interface
class Subject
{
public:
  virtual void addObserver(Observer *observer) {};
  virtual void removeObserver(Observer *observer) {};
  virtual void notifyObservers() {};
  virtual ~Subject() {}
};
//============================================================================================

#endif // _PAT_ObserverSubject_H
