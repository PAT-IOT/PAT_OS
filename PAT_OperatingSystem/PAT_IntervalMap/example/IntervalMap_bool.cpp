#include <Arduino.h>
#include "../src/PAT_IntervalMap.h"

//-------------------------------------------------------------------------------------------
// Test functions
void test_initial_device_state()
{
    IntervalMap<int, bool> deviceState(false); // Default state is OFF
    deviceState.print();                       // Initially, all values in the range will be OFF (device is off)
}

void test_single_device_state_interval()
{
    IntervalMap<int, bool> deviceState(false); // Default state is OFF
    deviceState.assign(10, 20, true);          // From time 10 to 20, device is ON
    deviceState.print();                       // Print the map state
}

void test_multiple_device_state_intervals()
{
    IntervalMap<int, bool> deviceState(false); // Default state is OFF
    deviceState.assign(10, 20, true);          // From time 10 to 20, device is ON
    deviceState.assign(20, 30, false);         // From time 20 to 30, device is OFF
    deviceState.assign(30, 40, true);          // From time 30 to 40, device is ON
    deviceState.print();                       // Print the map state after multiple intervals
}

void test_overlapping_device_state_intervals()
{
    IntervalMap<int, bool> deviceState(false); // Default state is OFF
    deviceState.assign(10, 20, true);          // From time 10 to 20, device is ON
    deviceState.assign(15, 25, false);         // Overlapping range: From time 15 to 25, device is OFF
    deviceState.print();                       // Print the map state after overlapping intervals
}

void test_remove_redundant_device_state_intervals()
{
    IntervalMap<int, bool> deviceState(false); // Default state is OFF
    deviceState.assign(10, 20, true);          // From time 10 to 20, device is ON
    deviceState.assign(15, 20, false);         // From time 15 to 20, device is OFF (removes redundant ON state)
    deviceState.print();                       // Print the map state after removal of redundant intervals
}

void test_invalid_device_state_interval()
{
    IntervalMap<int, bool> deviceState(false); // Default state is OFF
    deviceState.assign(30, 20, true);          // Invalid range: Start time is after end time
    deviceState.print();                       // Print the map state (no change)
}

//-------------------------------------------------------------------------------------------
// Main function to run all tests
void setup(void)
{
    Serial.begin(115200);
    delay(1000);

    Serial.print("test_initial_device_state\n");
    test_initial_device_state();

    Serial.print("test_single_device_state_interval\n");
    test_single_device_state_interval();

    Serial.print("test_multiple_device_state_intervals\n");
    test_multiple_device_state_intervals();

    Serial.print("test_overlapping_device_state_intervals\n");
    test_overlapping_device_state_intervals();

    Serial.print("test_remove_redundant_device_state_intervals\n");
    test_remove_redundant_device_state_intervals();

    Serial.print("test_invalid_device_state_interval\n");
    test_invalid_device_state_interval();
}

void loop(void) {}
