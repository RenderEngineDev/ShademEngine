#pragma once

#include <map>
#include <vector>

struct Point
{
    float x;
    float y;
    float z;
};

struct GridCell
{
    Point vertex[8];
    float value[8];
};