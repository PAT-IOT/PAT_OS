#include <Arduino.h>
#include "../src/PAT_IntervalMap.h"

// Define time-based intervals for each zone power consumption
void setup()
{
    // Initialize serial communication for output
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    // Initialize power consumption management for zones
    IntervalMap<int, String> zone1PowerMap("Zone 1 Residential", "Normal");
    IntervalMap<int, String> zone2PowerMap("Zone 2 Industrial", "Normal");
    IntervalMap<int, String> zone3PowerMap("Zone 3 Commercial", "Normal");

    // Define time-based intervals for Zone 1 (Residential)
    // Night (10 PM - 6 AM)
    zone1PowerMap.assign(0, 5, "Normal");
    zone1PowerMap.assign(5, 1000, "High Power Consumption");

    // Day (6 AM - 10 PM)
    zone1PowerMap.assign(0, 10, "Normal");
    zone1PowerMap.assign(10, 1000, "High Power Consumption");

    // Define constant limits for Zone 2 (Industrial)
    zone2PowerMap.assign(0, 50, "Normal");
    zone2PowerMap.assign(50, 1000, "Overload");

    // Define time-based intervals for Zone 3 (Commercial)
    // Day (6 AM - 6 PM)
    zone3PowerMap.assign(0, 30, "Normal");
    zone3PowerMap.assign(30, 1000, "High Power Consumption");

    // Night (6 PM - 6 AM)
    zone3PowerMap.assign(0, 15, "Normal");
    zone3PowerMap.assign(15, 1000, "High Power Consumption");

    // Simulate power consumption readings
    int zone1Consumption[] = {3, 7, 12};   // 3 kWh (Night), 7 kWh (Day), 12 kWh (Day)
    int zone2Consumption[] = {20, 60, 40}; // 20 kWh, 60 kWh (Overload), 40 kWh
    int zone3Consumption[] = {10, 35, 5};  // 10 kWh (Day), 35 kWh (Day), 5 kWh (Night)

    // Simulate monitoring system's reading for each zone
    monitorZonePowerConsumption(zone1PowerMap, zone1Consumption, 3);
    monitorZonePowerConsumption(zone2PowerMap, zone2Consumption, 3);
    monitorZonePowerConsumption(zone3PowerMap, zone3Consumption, 3);
}

// Function to monitor power consumption for each zone
void monitorZonePowerConsumption(IntervalMap<int, String> &zoneMap, int *consumption, int size)
{
    for (int i = 0; i < size; i++)
    {
        String status = zoneMap[consumption[i]];
        Serial.print("Power Consumption: ");
        Serial.print(consumption[i]);
        Serial.print(" kWh - Status: ");
        Serial.println(status);

        // Take action if necessary based on the status
        if (status == "Overload")
        {
            Serial.println("Action: Trigger overload protection!");
        }
        if (status == "High Power Consumption")
        {
            Serial.println("Action: Send alert for high consumption!");
        }
    }
}

void loop()
{
    // No continuous looping needed for this example
}
