#include "PAT_IntervalMap.h"

// Constructor associates whole range of K with val
template <typename K, typename V>
IntervalMap<K, V>::IntervalMap(V const &val)
    : default_value_(val)
{
    interval_name_ = "IntervalMap";
}

// Constructor with name and default value
template <typename K, typename V>
IntervalMap<K, V>::IntervalMap(String name, V const &val)
    : interval_name_(name), default_value_(val)
{
}

// Get the interval name
template <typename K, typename V>
String const &IntervalMap<K, V>::getIntervalName() const
{
    return interval_name_;
}

// Set the interval name
template <typename K, typename V>
void IntervalMap<K, V>::setIntervalName(String const &name)
{
    interval_name_ = name;
}

// Assign value val to interval [keyBegin, keyEnd)
template <typename K, typename V>
void IntervalMap<K, V>::assign(K const &keyBegin, K const &keyEnd, V const &val)
{
    if (keyBegin >= keyEnd)
        return; // Ignore invalid intervals

    // Erase the old interval(s) that overlap with the new one
    auto itBegin = map_.lower_bound(keyBegin);
    auto itEnd = map_.lower_bound(keyEnd);

    // Remove the range that overlaps
    map_.erase(itBegin, itEnd);

    // Insert the new value at the beginning of the range
    map_[keyBegin] = val;

    // If the keyEnd is not at the boundary, we need to insert it with the default value
    if (itEnd != map_.end() && itEnd->first != keyEnd)
    {
        map_[keyEnd] = default_value_;
    }
}

// Look-up of the value associated with key
template <typename K, typename V>
V &IntervalMap<K, V>::operator[](K const &key)
{
    auto it = map_.upper_bound(key);
    if (it == map_.begin())
    {
        return default_value_;
    }
    else
    {
        return (--it)->second;
    }
}

// Look-up of the value associated with key (const version for use in const methods)
template <typename K, typename V>
V const &IntervalMap<K, V>::operator[](K const &key) const
{
    auto it = map_.upper_bound(key);
    if (it == map_.begin())
    {
        return default_value_;
    }
    else
    {
        return (--it)->second;
    }
}

// Overload operator[] to handle range assignment using a pair (keyBegin, keyEnd)
template <typename K, typename V>
V &IntervalMap<K, V>::operator[](std::pair<K, K> const &range)
{
    // This operator takes the pair and directly assigns a value for the entire range
    return assignRange(range.first, range.second);
}

// This function allows assigning a value to the range dynamically
template <typename K, typename V>
V &IntervalMap<K, V>::assignRange(K const &start, K const &end)
{
    // Return a reference to the value at the start of the range
    auto it = map_.lower_bound(start);
    if (it == map_.end() || it->first != start)
    {
        map_[start] = default_value_; // Initialize with default value if not found
    }
    return map_[start]; // Return reference to the value at the start of the range
}

// Assignment operator for deep copying
template <typename K, typename V>
IntervalMap<K, V> &IntervalMap<K, V>::operator=(const IntervalMap &other)
{
    if (this != &other)
    {                                          // Prevent self-assignment
        map_ = other.map_;                     // Copy the map
        default_value_ = other.default_value_; // Copy the default value
    }
    return *this;
}

// Method to print intervals
template <typename K, typename V>
void IntervalMap<K, V>::print(K k_start, K k_stop) const
{
    if (map_.empty())
    {
        Serial.print("No intervals to print.\n");
        return;
    }

    K minKey;
    K maxKey;

    if (k_start || k_stop)
    {
        minKey = k_start;
        maxKey = k_stop;
    }
    else
    {
        // Find the minimum and maximum keys in the map
        minKey = map_.begin()->first;
        maxKey = std::prev(map_.end())->first;
    }

    Serial.print("--------------------Intervals------------------\n");
    Serial.print(interval_name_ + "[");

    bool first = true;
    for (K k = minKey; k <= maxKey; k++)
    {
        if (this->operator[](k + 1) == this->operator[](k) && (k + 1) <= maxKey)
        {
            if (!first)
            {
                Serial.print(", ");
            }
            Serial.print(k);
            first = false;
        }
        else
        {
            if (!first)
            {
                Serial.print(", ");
            }
            Serial.print(k);
            Serial.print("] = ");
            Serial.println(this->operator[](k));

            if ((k + 1) <= maxKey)
            {
                Serial.print(interval_name_ + "[");
                first = true;
            }
        }
    }
    Serial.print("-----------------------------------------------\n");
}
