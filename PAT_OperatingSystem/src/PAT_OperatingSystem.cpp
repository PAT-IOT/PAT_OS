

#include "PAT_OperatingSystem.h"
//===============================================================================================================================================================================================
//                                                                                         / Configuration
//===============================================================================================================================================================================================
operatingSystem os;

Class_ESP operatingSystem::esp;

const char *os_db_json = R"({
    "wifi": {
        "enable": true,
        "dynamic": true,
        "name": "PAT_IOT",
        "password": "PAT_IOT123",
        "ip": "192.168.1.100",
        "gw": "192.168.1.1",
        "sn": "255.255.255.0",
        "ssids": [
            {
                "name": "PAT_IOT",
                "mac": "B6:11:74:86:64:A1",
                "rssi_db": -59,
                "rssi_percentage": "82%",
                "rssi_distance": 3,
                "encryption": "WPA/WPA2",
                "frequency_band": "2.4GHz"
            }
        ],
        "connections": [
            {
                "name": "PAT_IOT",
                "password": "PAT_IOT123"
            }
        ]
    },
    "hotspot": {
        "enable": true,
        "dynamic": true,
        "name": "NovaRealy",
        "password": "NovaRealy",
        "ip": "192.168.1.100",
        "gw": "192.168.1.1",
        "sn": "255.255.255.0",
        "isOn": false
    },"inputsOutputs": {
      "number_of_channels": 12,
      "channels": [
        {
          "channel": 1,
          "status": 0
        },
        {
          "channel": 2,
          "status": 0
        },
        {
          "channel": 3,
          "status": 0
        },
        {
          "channel": 4,
          "status": 0
        },
        {
          "channel": 5,
          "status": 0
        },
        {
          "channel": 6,
          "status": 0
        },
        {
          "channel": 7,
          "status": 0
        },
        {
          "channel": 8,
          "status": 0
        },
        {
          "channel": 9,
          "status": 0
        },
        {
          "channel": 10,
          "status": 0
        },
        {
          "channel": 11,
          "status": 0
        },
        {
          "channel": 12,
          "status": 0
        }
      ]
    }
})";
// Initialize the static JsonStorage object
JsonStorage operatingSystem::db("/os_db.json", os_db_json, strlen(os_db_json) * 2 + 1024);

void operatingSystem::init()
{
  Serial.begin(115200);
  delay(500);
  log(COLOR_GREEN, TEXT_BOLD, "--------------------------------------------------\n");
  log(COLOR_GREEN, TEXT_BOLD, "--                                              --\n");
  log(COLOR_GREEN, TEXT_BOLD, "--             PAT OS is Booting ...            --\n");
  log(COLOR_GREEN, TEXT_BOLD, "--             detected drivers                 --\n");
  log(COLOR_GREEN, TEXT_BOLD, "--                                              --\n");
  if (db.open() != LOADED_ERROR)
  {
    os.log(COLOR_GREEN, TEXT_BOLD, "--             detected database ...           --\n");
    // db.print();
  }
  else
  {
    log(COLOR_RED, TEXT_BOLD, "--             data Base is not loaded          --\n");
  };
  db.save();
  db.close();
  log(COLOR_GREEN, TEXT_BOLD, "--             Free Heap:%03dk                   --\n", ESP.getFreeHeap() / 1000);
  log(COLOR_GREEN, TEXT_BOLD, "--             PAT OS booted                    --\n");
  log(COLOR_GREEN, TEXT_BOLD, "--                                              --\n");
  log(COLOR_GREEN, TEXT_BOLD, "--------------------------------------------------\n");
}

String operatingSystem::randomKey(int tokenLength)
{
  const char charset[] PROGMEM = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$^&*()-_=+[]{}|;:',.<>?/~`";
  const int charsetLength = sizeof(charset) - 1;
  char token[tokenLength + 1]; // Static to retain memory after function returns
  for (int i = 0; i < tokenLength; ++i)
  {
    token[i] = charset[random(0, charsetLength)];
  }
  token[tokenLength] = '\0'; // Null-terminate the string

  return String(token);
}

void operatingSystem::initVerbose()
{
  init();
  Serial.println("------------------------OS Start------------------------");
  this->esp.resetReason();
  Serial.printf("Free heap: %d kb\n", ESP.getFreeHeap() / 1000);
  // esp_task_wdt_init(0, false); // 0 = Timeout in milliseconds, false = Don't enable watchdog
  esp_task_wdt_init(30, true); // 30 seconds timeout, panic if a task is not responsive
  disableCore0WDT();
  Serial.println("--------------------------------------------------------");
  Serial.flush();
  // disableCore1WDT();
  // queue.init();
}
//------------------------------------------------------------------
