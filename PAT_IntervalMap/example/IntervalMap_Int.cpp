#include <Arduino.h>
#include "../src/PAT_IntervalMap.h"

//-------------------------------------------------------------------------------------------
// Test functions
void test_initial_temperature()
{
    IntervalMap<int, int> tempLog(0); // Default temperature is 0
    tempLog.print();                  // Initially, all values in the range will be 0 (e.g., no temperature readings)
}

void test_single_temperature_interval()
{
    IntervalMap<int, int> tempLog(0); // Default temperature is 0
    tempLog[{10, 20}] = 25;           // From time 10 to 20, the temperature is 25°C
    tempLog.print();                  // Print the map state
}

void test_multiple_temperature_intervals()
{
    IntervalMap<int, int> tempLog(0); // Default temperature is 0
    tempLog[{10, 20}] = 25;           // From time 10 to 20, the temperature is 25°C
    tempLog[{20, 30}] = 30;           // From time 20 to 30, the temperature is 30°C
    tempLog[{30, 40}] = 28;           // From time 30 to 40, the temperature is 28°C
    tempLog.print();                  // Print the map state after multiple temperature intervals
}

void test_overlapping_temperature_intervals()
{
    IntervalMap<int, int> tempLog(0); // Default temperature is 0
    tempLog[{10, 20}] = 25;           // From time 10 to 20, the temperature is 25°C
    tempLog[{15, 25}] = 26;           // Overlapping range: From time 15 to 25, temperature rises to 26°C
    tempLog.print();                  // Print the map state after overlapping temperature intervals
}

void test_remove_redundant_temperature_intervals()
{
    IntervalMap<int, int> tempLog(0); // Default temperature is 0
    tempLog[{10, 20}] = 25;           // From time 10 to 20, the temperature is 25°C
    tempLog[{15, 20}] = 0;            // From time 15 to 20, temperature drops to 0°C (removes redundant temperature)
    tempLog.print();                  // Print the map state after removal of redundant temperature intervals
}

void test_invalid_temperature_interval()
{
    IntervalMap<int, int> tempLog(0); // Default temperature is 0
    tempLog[{30, 20}] = 28;           // Invalid range: Start time is after end time
    tempLog.print();                  // Print the map state (no change)
}

//-------------------------------------------------------------------------------------------
// Main function to run all tests
void setup(void)
{
    Serial.begin(115200);
    delay(1000);

    Serial.print("test_initial_temperature\n");
    test_initial_temperature();

    Serial.print("test_single_temperature_interval\n");
    test_single_temperature_interval();

    Serial.print("test_multiple_temperature_intervals\n");
    test_multiple_temperature_intervals();

    Serial.print("test_overlapping_temperature_intervals\n");
    test_overlapping_temperature_intervals();

    Serial.print("test_remove_redundant_temperature_intervals\n");
    test_remove_redundant_temperature_intervals();

    Serial.print("test_invalid_temperature_interval\n");
    test_invalid_temperature_interval();
}

void loop(void) {}
