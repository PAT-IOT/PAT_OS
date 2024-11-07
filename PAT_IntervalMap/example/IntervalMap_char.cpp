#include <Arduino.h>
#include <iostream>
#include <map>
#include <utility>
#include "../src/PAT_IntervalMap.h"

//-------------------------------------------------------------------------------------------
// Test functions
void test_initial_value()
{
    IntervalMap<int, char> imap('A');
    imap.print(); // Initially, all values in the range will be 'A'
}
//-------------------------------------------------------------------------------------------
void test_single_interval()
{
    IntervalMap<int, char> imap('A');
    imap[{1, 5}] = 'B'; // Assign 'B' to the range [1, 5)
    imap.print();       // Print the map state
}
//-------------------------------------------------------------------------------------------
void test_overlapping_intervals()
{
    IntervalMap<int, char> imap('A');
    imap[{1, 5}] = 'B'; // Assign 'B' to the range [1, 5)
    imap[{3, 7}] = 'C'; // Overlap: Assign 'C' to the range [3, 7)
    imap.print();       // Print the map state after overlapping intervals
}
//-------------------------------------------------------------------------------------------
void test_non_overlapping_intervals()
{
    IntervalMap<int, char> imap('A');
    imap[{1, 5}] = 'B';  // Assign 'B' to the range [1, 5)
    imap[{6, 10}] = 'C'; // Assign 'C' to the range [6, 10)
    imap.print();        // Print the map state after non-overlapping intervals
}
//-------------------------------------------------------------------------------------------
void test_same_value_overlap()
{
    IntervalMap<int, char> imap("imap", 'A');
    imap[{1, 5}] = 'B'; // Assign 'B' to the range [1, 5)
    imap[{3, 7}] = 'B'; // Same value overlap: Assign 'B' to the range [3, 7)
    imap.print();       // Print the map state after same value overlap
}
//-------------------------------------------------------------------------------------------
void test_removal_of_redundant_intervals()
{
    IntervalMap<int, char> imap('A');
    imap[{1, 5}] = 'B'; // Assign 'B' to the range [1, 5)
    imap[{3, 7}] = 'B'; // Overlap with 'B' in range [3, 7)
    imap[{2, 3}] = 'A'; // Remove redundant interval [2, 3) by assigning 'A'
    imap.print();       // Print the map state after removal of redundant intervals
}
//-------------------------------------------------------------------------------------------
void test_reverse_interval()
{
    IntervalMap<int, char> imap('A');
    imap[{5, 1}] = 'B'; // Invalid: Reverse interval, should do nothing
    imap[{3, 7}] = 'C'; // Assign 'C' to the range [3, 7)
    imap.print(20, 20); // Print the map state after reverse interval (no change)
}
//-------------------------------------------------------------------------------------------
void test_empty_interval()
{
    IntervalMap<int, char> imap("imap", 'A');
    imap[{1, 1}] = 'B';  // Empty interval, should do nothing
    imap[{5, 10}] = 'C'; // Assign 'C' to the range [5, 10)
    imap.print(-20, 20); // Print the map state after empty interval
}
//-------------------------------------------------------------------------------------------
// Main function to run all tests
void setup(void)
{
    Serial.begin(115200);
    delay(1000);
    Serial.print("test_initial_value\n");
    test_initial_value();
    Serial.print("test_single_interval\n");
    test_single_interval();
    Serial.print("test_overlapping_intervals\n");
    test_overlapping_intervals();
    Serial.print("test_non_overlapping_intervals\n");
    test_non_overlapping_intervals();
    Serial.print("test_same_value_overlap\n");
    test_same_value_overlap();
    Serial.print("test_removal_of_redundant_intervals\n");
    test_removal_of_redundant_intervals();
    Serial.print("test_reverse_interval\n");
    test_reverse_interval();
    Serial.print("test_empty_interval\n");
    test_empty_interval();
}
void loop(void) {}
