#include "AttributeHelper.h"

#include <array>

struct AttributeData
{
    AttributeData();

    std::array<std::pair<const char*, AttributeType>, (std::size_t)AttributeType::Count> attributes;
};

AttributeData::AttributeData()
{
    attributes = std::array<std::pair<const char*, AttributeType>, (std::size_t)AttributeType::Count>
    {{
        { "Position", AttributeType::Position },
        { "Color", AttributeType::Color }
    }};
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
            return pair.first;
    }
}