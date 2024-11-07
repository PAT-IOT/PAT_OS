#include <Arduino.h>
#include "../src/PAT_IntervalMap.h"

// Create interval maps for each parameter
IntervalMap<int, String> temperatureMap("Temperature", "Unknown");
IntervalMap<int, String> pressureMap("Pressure", "Unknown");
IntervalMap<int, String> vibrationMap("Vibration", "Unknown");

// Function to initialize the interval maps with complex overlapping ranges
void initializeMaps()
{
    // Temperature ranges
    temperatureMap[{-40, 0}] = "Freezing";    // Interval [-40, 0)
    temperatureMap[{0, 15}] = "Cooling";      // Interval [0, 15)
    temperatureMap[{15, 25}] = "Normal";      // Interval [15, 25)
    temperatureMap[{25, 35}] = "Heating";     // Interval [25, 35)
    temperatureMap[{35, 50}] = "Overheating"; // Interval [35, 50)

    // Pressure ranges
    pressureMap[{0, 50}] = "Low Pressure";         // Interval [0, 50)
    pressureMap[{50, 100}] = "Normal";             // Interval [50, 100)
    pressureMap[{100, 150}] = "High Pressure";     // Interval [100, 150)
    pressureMap[{150, 200}] = "Critical Pressure"; // Interval [150, 200)

    // Vibration ranges (vibration frequency in Hz)
    vibrationMap[{0, 10}] = "Stable";            // Interval [0, 10)
    vibrationMap[{10, 20}] = "Minor Vibration";  // Interval [10, 20)
    vibrationMap[{20, 30}] = "High Vibration";   // Interval [20, 30)
    vibrationMap[{30, 40}] = "Severe Vibration"; // Interval [30, 40)
}

// Function to simulate sensor reading and print combined machine state
void checkMachineStatus(int temp, int pressure, int vibration)
{
    String tempStatus = temperatureMap[temp];
    String pressureStatus = pressureMap[pressure];
    String vibrationStatus = vibrationMap[vibration];

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("°C - Status: ");
    Serial.print(tempStatus);

    Serial.print(" | Pressure: ");
    Serial.print(pressure);
    Serial.print(" kPa - Status: ");
    Serial.print(pressureStatus);

    Serial.print(" | Vibration: ");
    Serial.print(vibration);
    Serial.print(" Hz - Status: ");
    Serial.println(vibrationStatus);

    // Combine the statuses for more advanced logic
    if (tempStatus == "Overheating" && pressureStatus == "High Pressure" && vibrationStatus == "Severe Vibration")
    {
        Serial.println("ALERT: Machine is in critical condition! Immediate action required.");
    }
    else if ((tempStatus == "Heating" || tempStatus == "Overheating") && (pressureStatus == "High Pressure" || pressureStatus == "Critical Pressure"))
    {
        Serial.println("WARNING: Machine under high stress, monitor closely.");
    }
    else if (tempStatus == "Cooling" && pressureStatus == "Low Pressure" && vibrationStatus == "Stable")
    {
        Serial.println("Machine is in safe, low-stress operational mode.");
    }
    else
    {
        Serial.println("Machine operational within normal parameters.");
    }
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    // Initialize the interval maps
    initializeMaps();

    // Print out the interval mappings for verification
    temperatureMap.print(-10, 50);
    pressureMap.print(0, 200);
    vibrationMap.print(0, 40);

    // Simulated sensor readings
    checkMachineStatus(30, 120, 35); // Example of critical alert
    checkMachineStatus(10, 70, 5);   // Example of safe mode
    checkMachineStatus(20, 85, 15);  // Example of normal operation
}

void loop()
{
    // No continuous looping needed for this example
}
