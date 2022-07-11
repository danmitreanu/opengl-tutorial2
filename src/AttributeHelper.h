#pragma once

#include <array>

enum class AttributeType
{
    Position = 0,
    Color,

    Count
};

struct AttributeData
{
    std::array<std::pair<const char*, AttributeType>, (std::size_t)AttributeType::Count> attributes;

    bool initialized = false;
    void initialize();
};

struct AttributeHelper
{
    static AttributeData& get_attributes();

    static const char* get_name(AttributeType);
};