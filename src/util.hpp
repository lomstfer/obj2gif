#pragma once
#include <iostream>

#define Log(x) std::cout << x << std::endl

namespace util {   
    inline float roundf(float i)
    {
        return i < 0 ? int(i - 0.5f) : int(i + 0.5f);
    }
    inline int roundftoi(float i)
    {
        return i < 0 ? int(i - 0.5f) : int(i + 0.5f);
    }
    
    inline float remap(float value, float from0, float from1, float to0, float to1)
    {
        float k = (value - from0) / (from1 - from0);
        return to0 + (to1 - to0) * k;
    }
}