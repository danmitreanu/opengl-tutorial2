#pragma once

#include <array>

enum class AttributeType
{
    Position = 0,
    Color,
    UV,
    Height,

    Count
};

struct AttributeHelper
{
    static const char* get_name(AttributeType);
    static AttributeType get_type(const char*);
};
