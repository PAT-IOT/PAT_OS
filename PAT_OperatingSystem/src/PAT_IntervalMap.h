#ifndef PAT_INTERVALMAP_H
#define PAT_INTERVALMAP_H

#include <Arduino.h>
#include <map>
#include <utility> // for std::pair

template <typename K, typename V>
class IntervalMap
{
      std::map<K, V> map_;
      V default_value_; // Default value for keys outside any intervals
      String interval_name_;

public:
      // Constructor associates whole range of K with val
      IntervalMap(V const &val);
      IntervalMap(String name, V const &val);

      // Get the interval name
      String const &getIntervalName() const;

      // Set the interval name
      void setIntervalName(String const &name);

      // Assign value val to interval [keyBegin, keyEnd)
      void assign(K const &keyBegin, K const &keyEnd, V const &val);

      // Look-up of the value associated with key
      V &operator[](K const &key);

      // Look-up of the value associated with key (const version for use in const methods)
      V const &operator[](K const &key) const;

      // Overload operator[] to handle range assignment using a pair (keyBegin, keyEnd)
      V &operator[](std::pair<K, K> const &range);

      // This function allows assigning a value to the range dynamically
      V &assignRange(K const &start, K const &end);

      // Assignment operator for deep copying
      IntervalMap &operator=(const IntervalMap &other);

      // Method to print intervals
      void print(K k_start = 0, K k_stop = 0) const;
};

#endif // PAT_INTERVALMAP_H
