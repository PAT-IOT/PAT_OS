#include <Arduino.h>
#include "../src/PAT_IntervalMap.h"
void setup()
{
    // Initialize serial communication for output
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    // Create an IntervalMap to map temperature ranges to status strings
    IntervalMap<int, String> temperatureMap("TemperatureMap", "Unknown");

    // Assign intervals for various temperature statuses
    temperatureMap.assign(-100, 0, "Critical Low");  // -100°C to 0°C
    temperatureMap.assign(0, 50, "Normal");          // 0°C to 50°C
    temperatureMap.assign(50, 75, "Warning");        // 50°C to 75°C
    temperatureMap.assign(75, 100, "Critical High"); // 75°C to 100°C
    temperatureMap.assign(100, 200, "Overheat");     // 100°C to 200°C

    // Print the defined intervals for verification
    temperatureMap.print(-50, 150);

    // Simulate a series of temperature readings and check the status for each
    int simulatedReadings[] = {-20, 10, 55, 78, 105, 130};

    for (int temp : simulatedReadings)
    {
        String status = temperatureMap[temp];
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.print("°C - Status: ");
        Serial.println(status);
    }
}

void loop()
{
    // No continuous looping needed for this example
}
