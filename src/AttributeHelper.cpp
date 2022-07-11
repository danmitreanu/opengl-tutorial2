#include "AttributeHelper.h"

#include <array>

void AttributeData::initialize()
{
    attributes = std::array<std::pair<const char*, AttributeType>, (std::size_t)AttributeType::Count>
    {{
        { "Position", AttributeType::Position },
        { "Color", AttributeType::Color }
    }};
}

AttributeData& AttributeHelper::get_attributes()
{
    static AttributeData data;
    if (!data.initialized)
    {
        data.initialize();
        data.initialized = true;
    }

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