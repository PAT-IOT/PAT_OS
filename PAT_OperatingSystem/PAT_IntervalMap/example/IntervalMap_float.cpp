#include <Arduino.h>
#include "../src/PAT_IntervalMap.h"

//-------------------------------------------------------------------------------------------
// Test functions
void test_empty_intervals()
{
    IntervalMap<float, float> tempMap(0.0f); // Default temperature is 0.0°C
    tempMap.print();                         // Initially, all values in the range will be 0.0°C
}

void test_single_interval()
{
    IntervalMap<float, float> tempMap(0.0f); // Default temperature is 0.0°C
    tempMap[{20.5f, 22.0f}] = 21.0f;         // Assign a temperature of 21.0°C to the range [20.5, 22.0)
    tempMap.print();                         // Print the map state
}

void test_multiple_intervals()
{
    IntervalMap<float, float> tempMap(0.0f); // Default temperature is 0.0°C
    tempMap[{15.0f, 18.0f}] = 16.5f;         // Assign 16.5°C to the range [15.0, 18.0)
    tempMap[{20.0f, 23.0f}] = 21.5f;         // Assign 21.5°C to the range [20.0, 23.0)
    tempMap.print();                         // Print the map state with multiple intervals
}

void test_removing_overlapping_intervals()
{
    IntervalMap<float, float> tempMap(0.0f); // Default temperature is 0.0°C
    tempMap[{18.0f, 22.0f}] = 19.0f;         // Assign 19.0°C to the range [18.0, 22.0)
    tempMap[{21.5f, 24.0f}] = 22.5f;         // Overlapping interval, should replace 19.0°C with 22.5°C in the overlapping range
    tempMap.print();                         // Print the map state after removal of overlapping intervals
}

void test_reverse_interval()
{
    IntervalMap<float, float> tempMap(0.0f); // Default temperature is 0.0°C
    tempMap[{25.0f, 20.0f}] = 18.0f;         // Invalid reverse interval, should do nothing
    tempMap.print();                         // Print the map state (no change)
}

void test_empty_interval()
{
    IntervalMap<float, float> tempMap(0.0f); // Default temperature is 0.0°C
    tempMap[{22.0f, 22.0f}] = 25.0f;         // Empty interval, should do nothing
    tempMap.print();                         // Print the map state (no change)
}

//-------------------------------------------------------------------------------------------
// Main function to run all tests
void setup(void)
{
    Serial.begin(115200);
    delay(1000);

    Serial.print("test_empty_intervals\n");
    test_empty_intervals();

    Serial.print("test_single_interval\n");
    test_single_interval();

    Serial.print("test_multiple_intervals\n");
    test_multiple_intervals();

    Serial.print("test_removing_overlapping_intervals\n");
    test_removing_overlapping_intervals();

    Serial.print("test_reverse_interval\n");
    test_reverse_interval();

    Serial.print("test_empty_interval\n");
    test_empty_interval();
}

void loop(void) {}
