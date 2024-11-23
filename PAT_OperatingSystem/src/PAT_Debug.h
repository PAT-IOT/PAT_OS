// version 021010

#ifndef _PAT_DEBUG_H
#define _PAT_DEBUG_H
#include <Arduino.h>
#include "esp_log.h"

// #define PAT_Debug
#define PAT_LOG
#ifdef PAT_Debug
extern unsigned long start_time_MEASURE;
extern unsigned long end_time_MEASURE;
extern unsigned long start_time_MEASURE0;
extern unsigned long end_time_MEASURE0;
String limitString(const String &str, size_t maxLength);
//----------------------------------------------------------------------------------------------------
#define DEBUG_MEASURE_TIME(FUNC_X)                                                             \
  start_time_MEASURE = millis();                                                               \
  FUNC_X                                                                                       \
  end_time_MEASURE = millis();                                                                 \
  Serial.println("________________________________________________________________________");  \
  Serial.print(start_time_MEASURE);                                                            \
  Serial.print(" : Execution time of Line ");                                                  \
  Serial.print(__LINE__);                                                                      \
  Serial.print(":");                                                                           \
  Serial.print(end_time_MEASURE - start_time_MEASURE);                                         \
  Serial.print(" ms into core:");                                                              \
  Serial.println(xPortGetCoreID());                                                            \
  Serial.println(limitString(#FUNC_X, 50));                                                    \
  Serial.println("_________________________________________________________________________"); \
  Serial.flush();
//----------------------------------------------------------------------------------------------------
#define DEBUG_MEASURE_TIME0(FUNC_X)                                                            \
  start_time_MEASURE0 = millis();                                                              \
  FUNC_X                                                                                       \
  end_time_MEASURE0 = millis();                                                                \
  Serial.println("________________________________________________________________________");  \
  Serial.println(start_time_MEASURE0);                                                         \
  Serial.print(" : Execution time of Line ");                                                  \
  Serial.print(__LINE__);                                                                      \
  Serial.print(":");                                                                           \
  Serial.print(end_time_MEASURE0 - start_time_MEASURE0);                                       \
  Serial.print(" ms into core:");                                                              \
  Serial.println(xPortGetCoreID());                                                            \
  Serial.println(limitString(#FUNC_X, 50));                                                    \
  Serial.println("_________________________________________________________________________"); \
  Serial.flush();
//----------------------------------------------------------------------------------------------------
#define DEBUG_EXECUTE_(FUNC_X) FUNC_X
//----------------------------------------------------------------------------------------------------

#define Debug_print(...)     \
  Serial.print(__VA_ARGS__); \
  Serial.flush()
#define Debug_println(...)     \
  Serial.println(__VA_ARGS__); \
  Serial.flush()
#define Debug_printf(...)     \
  Serial.printf(__VA_ARGS__); \
  Serial.flush()

#else
#define DB_print(FUNC_X)
#define DB_println(FUNC_X)
#define DEBUG_MEASURE_TIME(func) func
#define DEBUG_MEASURE_TIME0(func) func
#define DEBUG_EXECUTE(FUNC_X)
#endif
//-----------------------------------------------------------------------------------------------------------------
#ifdef PAT_LOG
// Define ANSI color codes and formatting
#define COLOR_RESET "\033[0m"
#define COLOR_BLACK "\033[30m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_LIGHT_GRAY "\033[37;1m"
#define COLOR_DARK_GRAY "\033[90m"
#define COLOR_LIGHT_RED "\033[91m"
#define COLOR_LIGHT_GREEN "\033[92m"
#define COLOR_LIGHT_YELLOW "\033[93m"
#define COLOR_LIGHT_BLUE "\033[94m"
#define COLOR_LIGHT_MAGENTA "\033[95m"
#define COLOR_LIGHT_CYAN "\033[96m"
#define COLOR_LIGHT_WHITE "\033[97m"
#define COLOR_ORANGE "\033[38;5;214m"
#define COLOR_PURPLE "\033[38;5;129m"
#define COLOR_TEAL "\033[38;5;37m"
#define COLOR_BROWN "\033[38;5;94m"
#define COLOR_PINK "\033[38;5;200m"
#define COLOR_GRAY "\033[90m"

// Additional text styles
#define TEXT_BOLD "\033[1m"
#define TEXT_UNDERLINE "\033[4m"
#define TEXT_BLINK "\033[5m"
#define TEXT_REVERSE "\033[7m"
#define TEXT_INVISIBLE "\033[8m"
#define TEXT_STRIKETHROUGH "\033[9m"
#define TEXT_DOUBLE_UNDERLINE "\033[21m"
#define TEXT_FRAKTUR "\033[20m"
#define TEXT_DIM "\033[2m"
#define TEXT_NORMAL "\033[0m"
// Log levels
enum LogLevel
{
  TRACE,
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  FATAL
};

//-----------------------------------------------------------------------------------------------------------------
class Class_Log
{
  String message;
  bool logon;

protected:
  bool isLogOn()
  {
    return logon;
  }
  void setLogOn()
  {
    logon = true;
  }

  void setLogOff()
  {
    logon = true;
  }

public:
  // Log function with formatting options
  Class_Log(const char *color = COLOR_MAGENTA, const char *styles = TEXT_REVERSE, const char *format = "[log]: ", ...); // Removed 'const'
  void init(const char *color, const char *styles, const char *format, ...);
  void deInit(void);

  void log(const char *color, const char *styles, const char *format, ...) const;
  void log(const char *color, const char *format, ...) const;
  void log(const char *format, ...) const;
  void log(LogLevel level, const char *format, ...) const;
  //-----------------------------------

  virtual void logOn(String name = "")
  {
    init(COLOR_MAGENTA, TEXT_BOLD, "[%s]:", name.c_str());
    logon = true;
  }
  
  virtual void logOn(void)
  {
    logon = true;
  }
   virtual void logOff(void)
  {
    logon = false;
    deInit();
  }
};
//-----------------------------------------------------------------------------------------------------------------
#else
// Define ANSI color codes and formatting
#define COLOR_RESET ""
#define COLOR_BLACK ""
#define COLOR_RED ""
#define COLOR_GREEN ""
#define COLOR_YELLOW ""
#define COLOR_BLUE ""
#define COLOR_MAGENTA ""
#define COLOR_CYAN ""
#define COLOR_WHITE ""
#define COLOR_LIGHT_GRAY ""
#define COLOR_DARK_GRAY ""
#define COLOR_LIGHT_RED ""
#define COLOR_LIGHT_GREEN ""
#define COLOR_LIGHT_YELLOW ""
#define COLOR_LIGHT_BLUE ""
#define COLOR_LIGHT_MAGENTA ""
#define COLOR_LIGHT_CYAN ""
#define COLOR_LIGHT_WHITE ""
#define COLOR_ORANGE ""
#define COLOR_PURPLE ""
#define COLOR_TEAL ""
#define COLOR_BROWN ""
#define COLOR_PINK ""
// Additional text styles
#define TEXT_BOLD ""
#define TEXT_UNDERLINE ""
#define TEXT_BLINK ""
#define TEXT_REVERSE ""
#define TEXT_INVISIBLE ""
#define TEXT_STRIKETHROUGH ""
#define TEXT_DOUBLE_UNDERLINE ""
#define TEXT_FRAKTUR ""
#define TEXT_DIM ""
#define TEXT_NORMAL ""
// Log levels
#define LogLevel uint8_t
#define TRACE 0
#define DEBUG 0
#define INFO 0
#define WARNING 0
#define ERROR 0
#define FATAL 0

class Class_Log
{
public:
  Class_Log(const char *color = "", const char *stylesr = "", const char *formatr = "", ...) {}
  void init(const char *color, const char *styles, const char *format, ...) {}
  void log(const char *color, const char *styles, const char *format, ...) {}
  void log(const char *color, const char *format, ...) {}
  void log(const char *format, ...) {}
  void log(LogLevel level, const char *format, ...) const {}
};

#endif

#endif //_PAT_DEBUG_H
       // extern Class_Log log;