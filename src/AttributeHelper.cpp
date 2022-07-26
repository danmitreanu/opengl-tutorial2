#include "AttributeHelper.h"

#include <array>
#include <string>
#include <string_view>
#include <cstring>

#include "utils/magic_enum.hpp"

struct AttributeData
{
    AttributeData();

    std::array<std::pair<std::string, AttributeType>, (std::size_t)AttributeType::Count> attributes;
};

AttributeData::AttributeData()
{
    /*attributes = std::array<std::pair<const char*, AttributeType>, (std::size_t)AttributeType::Count>
    {{
        { "Position", AttributeType::Position },
        { "Color", AttributeType::Color },
        { "UV", AttributeType::UV },
        { "Height", AttributeType::Height },
        { "Normal", AttributeType::Normal }
    }};*/

    magic_enum::enum_for_each<AttributeType>([this] (auto val)
    {
        constexpr AttributeType attr = val;
        int index = magic_enum::enum_integer(attr);
        if (index == (int)AttributeType::Count)
            return;

        auto name = magic_enum::enum_name(attr);

        this->attributes[index] = std::pair<std::string, AttributeType>(name, attr);
    });
}

AttributeData& get_attributes()
{
    static AttributeData data;

    return data;
}

const char* AttributeHelper::get_name(AttributeType attribute)
{
    for (const auto& pair : get_attributes().attributes)
    {
        if (pair.second == attribute)
            return pair.first.c_str();
    }
}

AttributeType AttributeHelper::get_type(const char* name)
{
    for (const auto& pair : get_attributes().attributes)
    {
        if (pair.first == name)
            return pair.second;
    }
 
    return AttributeType::Count;
}
