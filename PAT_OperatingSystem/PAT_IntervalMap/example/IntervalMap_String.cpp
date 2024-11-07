#include <Arduino.h>
#include "../src/PAT_IntervalMap.h"

//-------------------------------------------------------------------------------------------
// Test functions
void test_initial_events()
{
    IntervalMap<int, String> eventLog("SystemEvents", "Idle");
    eventLog.print(); // Initially, all values in the range will be "Idle"
}

void test_single_event_interval()
{
    IntervalMap<int, String> eventLog("SystemEvents", "Idle");
    eventLog[{10, 20}] = "System Initialized"; // Assign "System Initialized" from time 10 to 20
    eventLog.print();                          // Print the map state
}

void test_multiple_event_intervals()
{
    IntervalMap<int, String> eventLog("SystemEvents", "Idle");
    eventLog[{10, 20}] = "System Initialized"; // From time 10 to 20, the system is initialized
    eventLog[{20, 30}] = "Error Occurred";     // From time 20 to 30, an error occurred
    eventLog[{30, 40}] = "System Shutdown";    // From time 30 to 40, the system shuts down
    eventLog.print();                          // Print the map state after multiple events
}

void test_overlapping_event_intervals()
{
    IntervalMap<int, String> eventLog("SystemEvents", "Idle");
    eventLog[{10, 20}] = "System Initialized"; // From time 10 to 20, the system is initialized
    eventLog[{15, 25}] = "Error Occurred";     // Overlapping range: An error occurred from time 15 to 25
    eventLog.print();                          // Print the map state with overlapping intervals
}

void test_remove_redundant_event_intervals()
{
    IntervalMap<int, String> eventLog("SystemEvents", "Idle");
    eventLog[{10, 20}] = "System Initialized"; // From time 10 to 20, system initialized
    eventLog[{15, 20}] = "Idle";               // From time 15 to 20, system becomes idle (removes redundant event)
    eventLog.print();                          // Print the map state after removal of redundant events
}

void test_invalid_event_interval()
{
    IntervalMap<int, String> eventLog("SystemEvents", "Idle");
    eventLog[{30, 20}] = "System Shutdown"; // Invalid range: Start time is after end time
    eventLog.print();                       // Print the map state (no change)
}

//-------------------------------------------------------------------------------------------
// Main function to run all tests
void setup(void)
{
    Serial.begin(115200);
    delay(1000);

    Serial.print("test_initial_events\n");
    test_initial_events();

    Serial.print("test_single_event_interval\n");
    test_single_event_interval();

    Serial.print("test_multiple_event_intervals\n");
    test_multiple_event_intervals();

    Serial.print("test_overlapping_event_intervals\n");
    test_overlapping_event_intervals();

    Serial.print("test_remove_redundant_event_intervals\n");
    test_remove_redundant_event_intervals();

    Serial.print("test_invalid_event_interval\n");
    test_invalid_event_interval();
}

void loop(void) {}
