#pragma once

#include <array>

enum class Uniform
{
    Offset = 0,

    Count
};

struct UniformData
{
    std::array<std::pair<const char*, Uniform>, (std::size_t)Uniform::Count> uniforms;

    void initialize();
};

struct UniformHelper
{
public:
    static UniformData& get_uniforms();

    static const char* get_name(Uniform);
};