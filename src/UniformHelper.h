#pragma once

#include <array>

struct UniformData;

enum class Uniform
{
    None = 0,
    Offset,
    Model,
    View,
    Projection,
    MVP,
    Texture0,
    Texture1,
    Texture2,
    Texture3,
    Texture4,
    SplatMapTexture,

    Count
};

struct UniformHelper
{
public:
    static const char* get_name(Uniform);
};