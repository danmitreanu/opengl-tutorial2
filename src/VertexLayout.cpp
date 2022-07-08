#include "OpenGL.h"
#include <iostream>
#include <vector>

#include "VertexLayout.h"
#include "VertexAttribute.h"

void VertexLayout::AddVertexAttribute(AttributeType type, const std::size_t number_of_floats)
{
    VertexAttribute attrib;
    attrib.type = type;
    attrib.number_of_floats = number_of_floats;

    m_Attributes.push_back(attrib);
}

std::size_t VertexLayout::size() const
{
    std::size_t total_attribs = 0;
    for (auto a : m_Attributes)
        total_attribs += a.number_of_floats;

    return total_attribs * sizeof(float);
}

const std::vector<VertexAttribute>& VertexLayout::get_attributes() const
{
    return m_Attributes;
}
