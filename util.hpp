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