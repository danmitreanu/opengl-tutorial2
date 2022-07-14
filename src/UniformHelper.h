#pragma once

#include <array>

struct UniformData;

enum class Uniform
{
    Offset = 0,
    Model,
    View,
    Projection,
    MVP,
    Texture0,

    Count
};

struct UniformHelper
{
public:
    static const char* get_name(Uniform);
};