#pragma once

#include <array>

enum class Uniform
{
    Offset = 0,
    Model,
    View,
    Projection,
    MVP,

    Count
};

struct UniformData
{
    std::array<std::pair<const char*, Uniform>, (std::size_t)Uniform::Count> uniforms;

    bool initialized = false;
    void initialize();
};

struct UniformHelper
{
public:
    static UniformData& get_uniforms();

    static const char* get_name(Uniform);
};