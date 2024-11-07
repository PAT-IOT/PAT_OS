// version 020729
#include <Arduino.h>
#include "PAT_Debug.h"

unsigned long start_time_MEASURE;
unsigned long end_time_MEASURE;
unsigned long start_time_MEASURE0;
unsigned long end_time_MEASURE0;

String limitString(const String &str, size_t maxLength)
{
  if (str.length() <= maxLength)
  {
    return str;
  }
  else
  {
    String truncatedStr = str.substring(0, maxLength - 15);
    truncatedStr += "...";
    truncatedStr += str.substring(str.length() - 15);
    return truncatedStr;
  }
}

#ifdef PAT_LOG
Class_Log::Class_Log(const char *color, const char *styles, const char *format, ...)
{
  // Prepare a buffer to hold the formatted message
  char formattedMessage[256]; // Adjust size as necessary

  // Start processing variable arguments
  va_list args;
  va_start(args, format);

  // Format the message
  vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);

  // Reset the argument list
  va_end(args);

  this->message = String(styles) + String(color) + String(formattedMessage) + COLOR_RESET;
}

void Class_Log::init(const char *color, const char *styles, const char *format, ...)
{
  // Prepare a buffer to hold the formatted message
  char formattedMessage[256]; // Adjust size as necessary

  // Start processing variable arguments
  va_list args;
  va_start(args, format);

  // Format the message
  vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);

  // Reset the argument list
  va_end(args);

  this->message = String(styles) + String(color) + String(formattedMessage) + COLOR_RESET;
}
//-----------------------------------------------------------------------------------------------------------------------------
// Class_Log log;

void Class_Log::log(const char *color, const char *styles, const char *format, ...) const
{
  // Prepare a buffer to hold the formatted message
  char formattedMessage[256]; // Adjust size as necessary

  // Start processing variable arguments
  va_list args;
  va_start(args, format);

  // Format the message
  vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);

  // Reset the argument list
  va_end(args);

  // Create the final log message with color and styles
  String finalMessage = String(styles) + String(color) + String(formattedMessage) + COLOR_RESET;

  // Log the message
  log_printf("%s%s", message.c_str(), finalMessage.c_str());
}
void Class_Log::log(const char *color, const char *format, ...) const
{
  // Prepare a buffer to hold the formatted message
  char formattedMessage[256]; // Adjust size as necessary

  // Start processing variable arguments
  va_list args;
  va_start(args, format);

  // Format the message
  vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);

  // Reset the argument list
  va_end(args);

  // Create the final log message with color and styles
  String finalMessage = String(color) + String(formattedMessage) + COLOR_RESET;

  // Log the message
  log_printf("%s%s", message.c_str(), finalMessage.c_str());
}

void Class_Log::log(const char *format, ...) const
{
  // Prepare a buffer to hold the formatted message
  char formattedMessage[256]; // Adjust size as necessary

  // Start processing variable arguments
  va_list args;
  va_start(args, format);

  // Format the message
  vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);

  // Reset the argument list
  va_end(args);

  // Create the final log message with color and styles
  String finalMessage = String(formattedMessage);

  // Log the message
  log_printf("%s%s", message.c_str(), finalMessage.c_str());
}

void Class_Log::log(LogLevel level, const char *format, ...) const
{
  // Prepare the formatted message
  char formattedMessage[256]; // Adjust size as necessary

  // Start processing variable arguments
  va_list args;
  va_start(args, format);
  vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);
  va_end(args);

  // Determine the log level and call the appropriate log function
  switch (level)
  {
  case TRACE:
    log(COLOR_GRAY, TEXT_DIM, "TRACE -> %s", formattedMessage);
    break;
  case DEBUG:
    log(COLOR_WHITE, TEXT_NORMAL, "DEBUG -> %s", formattedMessage);
    break;
  case INFO:
    log(COLOR_BLUE, TEXT_BOLD, "INFO -> %s", formattedMessage);
    break;
  case WARNING:
    log(COLOR_YELLOW, TEXT_BOLD, "WARNING -> %s", formattedMessage);
    break;
  case ERROR:
    log(COLOR_RED, TEXT_BOLD, "ERROR -> %s", formattedMessage);
    break;
  case FATAL:
    log(COLOR_RED, TEXT_BLINK, "FATAL -> %s", formattedMessage); // Optional: Blinking for critical errors
    break;
  default:
    log(COLOR_RESET, "LOG -> %s", formattedMessage); // Default case for unknown levels
    break;
  }
}

#endif
//       void Class_Log::log(LogLevel level, const char *format, ...) const
// {

//   // Log the message based on the level
//   switch (level)
//   {
// #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_VERBOSE
//   case VERBOSE:
//     ESP_LOGV("Class_Log", "%s", formattedMessage.c_str());
//     break;
// #endif
// #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
//   case DEBUG:
//     ESP_LOGD("Class_Log", "%s", formattedMessage.c_str());
//     break;
// #endif
// #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_ERROR
//   case ERROR:
//     ESP_LOGE("Class_Log", "%s", formattedMessage.c_str());
//     break;
// #endif
// #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_WARN
//   case WARNING:
//     ESP_LOGW("Class_Log", "%s", formattedMessage.c_str());
//     break;
// #endif
// #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
//   case INFO:
//     ESP_LOGI("Class_Log", "%s", formattedMessage.c_str());
//     break;
// #endif
//   default:
// #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_NONE
//     ESP_LOGI("Class_Log", "Unknown log level: %s", formattedMessage.c_str());
//     break;
// #endif
//   }
// }
// Class_Log(COLOR_MAGENTA, TEXT_BOLD, "[os]: ")
//     {