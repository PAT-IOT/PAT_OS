

#include "PAT_JsonStorage.h"
//_____________________________________________________________________________________________________________________________________
// Load JSON from a given path
bool JsonStorage::loadFromFile(const char *path, int maxRetries)
{
  int attempts = 0;
  while (attempts <= maxRetries)
  {
    File file = SPIFFS.open(path, FILE_READ);
    if (!file)
    {
      Serial.println("Failed to open file, retrying...");
      attempts++;
      vTaskDelay(50 / portTICK_PERIOD_MS); // Small delay before retrying
    }
    else
    {
      DeserializationError error = deserializeJson(*jsonDoc, file);
      file.close();
      if (!error)
      {
        // Serial.printf("loaded %s\n", path);
        return true;
      }
      else
      {
        // Serial.printf("error failed to load %s\n", path);
        return false;
      }
    }
  }
  // Serial.printf("failed to load %s\n", path);
  return false;
}
//_____________________________________________________________________________________________________________________________________
// f.g. file/setting/wifi.json  ---->file/setting/wifi_backup.json
const char *JsonStorage::getBackupFilePath(const char *filePath) const
{
  size_t length = strlen(filePath);
  const char *dotPos = strrchr(filePath, '.');

  size_t baseNameLength = (dotPos != nullptr) ? (dotPos - filePath) : length;
  size_t extensionLength = (dotPos != nullptr) ? (length - baseNameLength) : 0;
  size_t backupLength = baseNameLength + strlen("_backup") + extensionLength;

  char *result = new char[backupLength + 1]; // +1 for null terminator

  strncpy(result, filePath, baseNameLength);
  strncpy(result + baseNameLength, "_backup", strlen("_backup"));

  if (dotPos != nullptr)
  {
    strncpy(result + baseNameLength + strlen("_backup"), dotPos, extensionLength);
  }

  result[backupLength] = '\0'; // Null-terminate the result

  return result;
}

//_____________________________________________________________________________________________________________________________________

// Function to get the backup file path
const char *JsonStorage::getNameFile(const char *filePath) const
{
  // Find the last '/' to isolate the filename
  const char *lastSlash = strrchr(filePath, '/');
  const char *fileName = (lastSlash) ? lastSlash + 1 : filePath;

  // Find the last '.' to separate the name from the extension
  const char *lastDot = strrchr(fileName, '.');
  size_t nameLength = (lastDot) ? (lastDot - fileName) : strlen(fileName);

  // Allocate memory for the name (including null terminator)
  char *name = new char[nameLength + 1];
  strncpy(name, fileName, nameLength);
  name[nameLength] = '\0'; // Null-terminate the string

  return name;
}
//_____________________________________________________________________________________________________________________________________

// Function to extract the name (stem) from the file path
String JsonStorage::getName(const char *filePath) const
{
  return String(*nameFile);
}

//_____________________________________________________________________________________________________________________________________

fileStatus JsonStorage::initDefaultJson()
{
  Serial.println("Initializing with default JSON...");
  DeserializationError error = deserializeJson(*jsonDoc, defaultJsonString);
  if (!error)
  {
    return LOADED_DEFAULT_FILE;
  }
  else
  {
    if (logon)
      log(COLOR_RED, TEXT_BOLD, "Failed to parse default JSON string:%s\n", error.c_str());
  }
  return LOADED_ERROR;
}
//_____________________________________________________________________________________________________________________________________
// Destructor
JsonStorage::~JsonStorage()
{
  if (jsonDoc != nullptr)
  {
    jsonDoc->clear();  // Free the allocated memory inside the document
    delete jsonDoc;    // Delete the DynamicJsonDocument object
    jsonDoc = nullptr; // Set the pointer to nullptr to avoid dangling pointers
  }
}
//_____________________________________________________________________________________________________________________________________
// Destructor
bool JsonStorage::close()
{
  openedStatus = LOADED_ERROR;
  if (jsonDoc != nullptr)
  {
    jsonDoc->clear();  // Free the allocated memory inside the document
    delete jsonDoc;    // Delete the DynamicJsonDocument object
    jsonDoc = nullptr; // Set the pointer to nullptr to avoid dangling pointers
  }
  if (logon)
    log(COLOR_GREEN, TEXT_NORMAL, "file closed\n");

  return (jsonDoc == nullptr);
}
//_____________________________________________________________________________________________________________________________________
// Initialize SPIFFS and create mutex, load or initialize JSON
bool JsonStorage::init()
{
  if (jsonDoc == nullptr)
  {
    jsonDoc = new DynamicJsonDocument((json_size + 1) * 2);
  }
  if (mutex == nullptr)
  {
    mutex = xSemaphoreCreateMutex(); // Initialize the mutex
  }
  if (!SPIFFS.begin(true))
  {
    Serial.println("Failed to mount SPIFFS");
    delay(500);
    if (!SPIFFS.begin(true))
    {
      Serial.println("Failed to mount SPIFFS again");
      delay(500);
      if (!SPIFFS.begin(true))
      {
        Serial.println("Failed to mount SPIFFS again");
        return false;
      }
    }
  }
  // Serial.println("Mounted SPIFFS");
  return true;
}
//_____________________________________________________________________________________________________________________________________
// Initialize SPIFFS and create mutex, load or initialize JSON
fileStatus JsonStorage::open()
{
  //------------------------------
  if (!init())
  {
    openedStatus = fileStatus::LOADED_ERROR;
    return openedStatus;
  }

  if (openedStatus != fileStatus::LOADED_FILE)
  {
    openedStatus = load();
    if (openedStatus != fileStatus::LOADED_FILE)
    {
      save();
    }
  }
  //------------------------------
  if (logon)
  {
    switch (openedStatus)
    {
    case LOADED_FILE:
      log(COLOR_GREEN, TEXT_NORMAL, "File opened successfully.\n");
      break;
    case LOADED_BACKUP_FILE:
      log(COLOR_GREEN, TEXT_NORMAL, "File could not be opened, using backup file.\n");
      break;
    case LOADED_DEFAULT_FILE:
      log(COLOR_YELLOW, TEXT_BOLD, "File could not be opened, using default file.\n");
      break;
    default:
      log(COLOR_RED, TEXT_BOLD, "Failed to open file, backup file, and default file.\n");
      break;
    }
  }

  return openedStatus;
}
//_____________________________________________________________________________________________________________________________________
fileStatus JsonStorage::load(int maxRetries)
{
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
  {
    fileStatus status = LOADED_ERROR;

    // Attempt to load from the primary file
    if (loadFromFile(filePath, maxRetries))
    {
      status = LOADED_FILE;
    }
    // Attempt to load from the backup file
    else if (loadFromFile(filePath_backup, maxRetries))
    {
      status = LOADED_BACKUP_FILE;
    }
    // Initialize default JSON if loading failed
    else
    {
      status = initDefaultJson();
    }

    xSemaphoreGive(mutex);
    return status;
  }

  return LOADED_ERROR;
}
//_____________________________________________________________________________________________________________________________________
bool JsonStorage::saveToFile(const char *path, const char *data)
{
  if (!this->init())
  {
    return false;
  }

  File file = SPIFFS.open(path, FILE_WRITE); // Open file for writing

  if (!file)
  {
    if (logon)
    {
      log(COLOR_RED, TEXT_NORMAL, "Failed to open file for writing path: %s\n", path);
    }
    return false;
  }

  if (file.print(data))
  {
    file.close();
    return true; // Successfully written
  }
  else
  {
    Serial.println("Write failed");
    file.close();
    return false; // Write failed
  }
}
//_____________________________________________________________________________________________________________________________________
bool JsonStorage::save()
{
  bool fileDataSuccess = false;
  bool fileData_backupSuccess = false;
  int attempts = 0;

  String fileData;

  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
  {
    size_t bytesWritten = serializeJsonPretty(*jsonDoc, fileData); // Correct return type
    if (bytesWritten < 1)
    {
      if (logon)
      {
        log(COLOR_RED, TEXT_NORMAL, "Error(saving) serializing JSON for %s\n", filePath);
      }
      xSemaphoreGive(mutex); // Release mutex before returning
      return false;
    }

    // Attempt to save to the primary file
    while (attempts <= 3 && !fileDataSuccess)
    {
      attempts++;
      fileDataSuccess = saveToFile(this->filePath, fileData.c_str());
    }

    // Attempt to save to the backup file
    attempts = 0;
    while (attempts <= 3 && !fileData_backupSuccess)
    {
      attempts++;
      fileData_backupSuccess = saveToFile(this->filePath_backup, fileData.c_str());
    }

    xSemaphoreGive(mutex); // Release the mutex after saving
    // Return true only if both saves were successful
    return fileDataSuccess && fileData_backupSuccess;
  }

  if (logon)
  {
    log(COLOR_RED, TEXT_NORMAL, "Failed to take mutex in save.");
  }
  return false;
}

//_____________________________________________________________________________________________________________________________________
DynamicJsonDocument &JsonStorage::json()
{
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
  {
    xSemaphoreGive(mutex);
    return *jsonDoc;
  }
  // Return a static empty document if mutex is not available
  static DynamicJsonDocument emptyDoc(1);
  return emptyDoc;
}
//_____________________________________________________________________________________________________________________________________
JsonVariant JsonStorage::operator[](const char *key)
{
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
  {
    xSemaphoreGive(mutex);
    return (*jsonDoc)[key];
  }
  // Return a static empty document if mutex is not available
  static DynamicJsonDocument emptyDoc(1);
  return (emptyDoc)[key];
}
//_____________________________________________________________________________________________________________________________________
void JsonStorage::print()
{
  if (xSemaphoreTake(mutex, 3000) == pdTRUE)
  {
    if (openedStatus != LOADED_ERROR) // opened
    {
      //----------------------------------
      String output;
      size_t bytesWritten = serializeJsonPretty(*jsonDoc, output); // Correct return type

      if (bytesWritten > 0) // Check if any bytes were written
      {
        Serial.println(output);
        Serial.flush();
      }
      else
      {
        if (logon)
        {
          log(COLOR_RED, TEXT_NORMAL, "print: Error serializing JSON for %s\n", filePath);
        }

        Serial.flush();
      }
      //----------------------------------
    }
    else
    {
      if (logon)
      {
        log(COLOR_RED, TEXT_NORMAL, "print:Error JSON is not opened%s\n", filePath);
      }
    }
    xSemaphoreGive(mutex);
  }
  else
  {
    if (logon)
    {
      log(COLOR_RED, TEXT_NORMAL,"Mutex error for %s\n", filePath);
    }
    Serial.flush();
  }
}

//_____________________________________________________________________________________________________________________________________
/**
 * @brief Finds the maximum numeric value for a specified key in a JSON array,
 *        supporting nested keys via dot notation (e.g., "accounts.balance").
 *
 * @param key The key to search for in the JSON array. Nested keys are specified with dot notation.
 * @return The maximum numeric value found, or -1 if:
 *         - The key does not exist.
 *         - The array is empty.
 *         - No numeric values are found at the specified key path.
 *
 * @note Assumes the JSON array is well-formed and that the key points to numeric data.
 *       For floating-point numbers, the maximum is determined based on standard comparison rules.
 *
 * @example
 * JsonStorage jsonStorage;
 *
 *  Example JSON structure:
 *  {
 *    "accounts": [
 *      {"id": 101, "balance": 5000.75},
 *      {"id": 102, "balance": 3200.50},
 *      {"id": 103, "balance": 1500.00}
 *    ]
 *  }
 *
 * int maxBalance = jsonStorage.max("accounts.balance");
 * Serial.println("Max balance: " + String(maxBalance)); // Output: 5000.75
 *
 * @limitations
 * - Dot notation does not support keys with embedded periods (e.g., "user.data.key").
 * - Nested arrays are not traversed.
 */
int JsonStorage::max(const char *key)
{
  // Split the key by '.' to handle nested structures, e.g., "accounts.id"
  String keyStr = key;
  int dotIndex = keyStr.indexOf('.');
  String topKey = dotIndex == -1 ? keyStr : keyStr.substring(0, dotIndex);
  String nestedKey = dotIndex == -1 ? "" : keyStr.substring(dotIndex + 1);

  // Check if the top-level key is an array
  JsonArray jsonArray;
  if (this->json().containsKey(topKey.c_str()) && this->json()[topKey.c_str()].is<JsonArray>())
  {
    jsonArray = this->json()[topKey.c_str()].as<JsonArray>();
  }
  else
  {
    return -1; // Return -1 if the specified top-level key is not a valid array
  }

  int maxValue = -1;

  // Loop through the JSON array and find the max value of the nested key
  for (JsonObject obj : jsonArray)
  {
    JsonVariant nestedValue = obj;
    if (!nestedKey.isEmpty())
    {
      int nextDotIndex;
      String subKey = nestedKey;
      while ((nextDotIndex = subKey.indexOf('.')) != -1)
      {
        String currentKey = subKey.substring(0, nextDotIndex);
        subKey = subKey.substring(nextDotIndex + 1);

        if (nestedValue.containsKey(currentKey.c_str()))
        {
          nestedValue = nestedValue[currentKey.c_str()];
        }
        else
        {
          nestedValue = JsonVariant();
          break;
        }
      }

      if (!nestedValue.containsKey(subKey.c_str()))
      {
        continue;
      }
      nestedValue = nestedValue[subKey.c_str()];
    }

    if (nestedValue.is<int>())
    {
      int value = nestedValue.as<int>();
      if (value > maxValue)
      {
        maxValue = value;
      }
    }
  }

  return maxValue; // Return the maximum value found
}
//_____________________________________________________________________________________________________________________________________
