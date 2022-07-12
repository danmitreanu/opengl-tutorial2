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

    Count
};

struct UniformHelper
{
public:
    static UniformData& get_uniforms();

    static const char* get_name(Uniform);
};