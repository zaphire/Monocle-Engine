#pragma once

// Class to represent a range of values, from low to high, inclusive of both.
// If low == high, the range is one value only.
template<typename T>
class Range
{
public:
    Range(T lowValue, T highValue): low(lowValue), high(highValue) {}
    Range(T singleValue): low(singleValue), high(singleValue) {}
    
    Range& operator=(const Range& range)
    {
        if (&range != this)
        {
            low = range.low;
            high = range.high;
        }
        return *this;
    }
    
    T low;
    T high;
};
