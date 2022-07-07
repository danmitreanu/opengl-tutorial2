#pragma once

#include <vector>

#include "VertexAttribute.h"
#include "AttributeHelper.h"

class VertexLayout
{
public:
    VertexLayout() = default;

    std::size_t size() const;

    void AddVertexAttribute(AttributeType, const std::size_t);

    const std::vector<VertexAttribute>& get_attributes() const;

private:
    std::vector<VertexAttribute> m_Attributes;
};
