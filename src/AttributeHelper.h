#pragma once

#include <array>

enum class AttributeType
{
    Position = 0,
    Color,

    Count
};

struct AttributeHelper
{
    static const char* get_name(AttributeType);
};