# PAT_IntervalMap Library

The `PAT_IntervalMap` library provides a flexible and efficient way to manage interval-based key-value mappings. It supports assigning values to key ranges and looking up values for individual keys, while also allowing for easy visualization of intervals.

## Features

- **Range Assignments**: Assign values to key intervals, with each key in the interval sharing the same value.
- **Efficient Lookups**: Retrieve the value for any key, returning a default value if no value is assigned to that key.
- **Interval Printing**: Print intervals and their associated values in an easily readable format.

## Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/PAT-IOT/PAT_IntervalMap.git
    ```

2. Copy the `PAT_IntervalMap` directory into your Arduino `libraries` folder.

## Example Usage

## expected Serial Output:

- **Heart Rate**:  55 bpm - Status: Bradycardia Alert.
- **SpO2 Level**: 92% - Status: Low Oxygen Alert.
- **ALERT**: Monitor patient closely; consider intervention.
- **Heart Rate**:  150 bpm - Status: Critical Tachycardia.
- **SpO2 Level**: 88% - Status: Critical Hypoxia Alert.
- **Blood Pressure**: 160/100 mmHg - Status: Critical Hypertension.
- **EMERGENCY**: Immediate medical intervention required!.
- **Heart Rate**:  72 bpm - Status: Normal Heart Rate.
- **SpO2 Level**: 89% - Status: Critical Hypoxia Alert.
- **Blood Pressure**: 140/90 mmHg - Status: Critical Hypertension.
- **EMERGENCY**: Immediate medical intervention required!.

```cpp

#include <Arduino.h>
#include "../src/PAT_IntervalMap.h"
// Create interval maps for each vital sign parameter
IntervalMap<int, String> heartRateMap("Unknown Status");
IntervalMap<int, String> spo2Map("Unknown Status");
IntervalMap<std::pair<int, int>, String> bloodPressureMap("Unknown Status");

// Initialize vital sign thresholds and corresponding alerts
void initializeMaps()
{
    // Heart rate in beats per minute (bpm)
    heartRateMap[{30, 60}] = "Bradycardia Alert";      // Very low heart rate
    heartRateMap[{60, 100}] = "Normal Heart Rate";     // Normal range
    heartRateMap[{100, 130}] = "Tachycardia Alert";    // Elevated heart rate
    heartRateMap[{130, 200}] = "Critical Tachycardia"; // Dangerously high heart rate

    // SpO₂ levels in percentage
    spo2Map[{85, 90}] = "Critical Hypoxia Alert"; // Immediate attention needed
    spo2Map[{90, 95}] = "Low Oxygen Alert";       // Below normal but less critical
    spo2Map[{95, 100}] = "Normal Oxygen Level";   // Healthy range

    // Blood pressure in systolic/diastolic mmHg using std::pair
    bloodPressureMap[{{80, 50}, {90, 60}}] = "Hypotension Alert";        // Low BP
    bloodPressureMap[{{90, 60}, {120, 80}}] = "Normal Blood Pressure";   // Normal BP
    bloodPressureMap[{{120, 80}, {140, 90}}] = "Hypertension Alert";     // High BP
    bloodPressureMap[{{140, 90}, {180, 120}}] = "Critical Hypertension"; // Dangerously high BP
}

// Function to simulate ICU monitoring and print out statuses
void monitorPatient(int heartRate, int spo2, std::pair<int, int> bloodPressure)
{
    String heartRateStatus = heartRateMap[heartRate];
    String spo2Status = spo2Map[spo2];
    String bloodPressureStatus = bloodPressureMap[bloodPressure];

    Serial.print("Heart Rate: ");
    Serial.print(heartRate);
    Serial.print(" bpm - Status: ");
    Serial.println(heartRateStatus);

    Serial.print("SpO2 Level: ");
    Serial.print(spo2);
    Serial.print("% - Status: ");
    Serial.println(spo2Status);

    Serial.print("Blood Pressure: ");
    Serial.print(bloodPressure.first);
    Serial.print("/");
    Serial.print(bloodPressure.second);
    Serial.print(" mmHg - Status: ");
    Serial.println(bloodPressureStatus);

    // Combining statuses for emergency response
    if (heartRateStatus == "Critical Tachycardia" || spo2Status == "Critical Hypoxia Alert" || bloodPressureStatus == "Critical Hypertension")
    {
        Serial.println("EMERGENCY: Immediate medical intervention required!");
    }
    else if (heartRateStatus == "Tachycardia Alert" || spo2Status == "Low Oxygen Alert" || bloodPressureStatus == "Hypertension Alert")
    {
        Serial.println("ALERT: Monitor patient closely; consider intervention.");
    }
    else if (heartRateStatus == "Normal Heart Rate" && spo2Status == "Normal Oxygen Level" && bloodPressureStatus == "Normal Blood Pressure")
    {
        Serial.println("Patient stable with no intervention required.");
    }
    else
    {
        Serial.println("Patient condition requires further evaluation.");
    }
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    // Initialize vital sign ranges in the maps
    initializeMaps();

    // Simulate patient readings
    monitorPatient(55, 92, {120, 80});   // Example: Normal patient
    monitorPatient(150, 88, {160, 100}); // Example: Critical alert
    monitorPatient(72, 89, {140, 90});   // Example: Alert level
}

void loop()
{
    // Real-time monitoring would be placed here for continuous updates
}

