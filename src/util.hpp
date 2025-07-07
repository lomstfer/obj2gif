#pragma once
#include <iostream>

#define Log(x) std::cout << x << std::endl

float roundf(float i)
{
    return i < 0 ? int(i - 0.5) : int(i + 0.5);
}
int roundftoi(float i)
{
    return i < 0 ? int(i - 0.5) : int(i + 0.5);
}

float remap(float value, float from0, float from1, float to0, float to1)
{
    float k = (value - from0) / (from1 - from0);
    return to0 + (to1 - to0) * k;
}