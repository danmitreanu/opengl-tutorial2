#pragma once

#include <vector>

#include "VertexAttribute.h"

// vertex layout.
class VertexLayout
{
    // this class describes the attributes you have in the data:
public:
    VertexLayout() = default;

    std::size_t size() const;
    //^ returns the size in bytes of a single vertex

    void AddVertexAttribute(const char* name, const std::size_t number_of_floats);

    const std::vector<VertexAttribute> get_attributes() const;

private:
    std::vector<VertexAttribute> m_Attributes;
};