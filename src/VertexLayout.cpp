#include "VertexLayout.h"
#include <OpenGL/gl3.h>
#include <iostream>
#include <vector>

void VertexLayout::AddVertexAttribute(const char* name, const std::size_t number_of_floats)
{
    VertexAttribute attrib;
    attrib.name = name;
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

VertexBuffer::VertexBuffer()
{
    // generate the buffer
    glGenVertexArrays(1, &m_Vao);

    glGenBuffers(1, &m_Buffer);
}

VertexBuffer::~VertexBuffer()
{
    if (m_Vao)
        glDeleteVertexArrays(1, &m_Vao);

    glDeleteBuffers(1, &m_Buffer);
}

const std::vector<VertexAttribute> VertexLayout::get_attributes() const
{
    return m_Attributes;
}

void VertexBuffer::create(const float* data, const VertexLayout& vertex_layout, std::size_t vertex_count)
{
    int attrib_count = vertex_layout.get_attributes().size();

    glBindVertexArray(m_Vao);

    // only one buffer per vertex including all attributes

    std::size_t offset = 0;

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_layout.size() * vertex_count, (void*)data, GL_STATIC_DRAW);

    // Create buffers for vertices attributes

    int index = 0;
    for (auto &attrib : vertex_layout.get_attributes())
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, attrib.number_of_floats, GL_FLOAT, GL_FALSE, 0, (void*)offset);

        offset += attrib.number_of_floats * sizeof(float);
        index++;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for (int i = 0; i < index; i++)
        glDisableVertexAttribArray(i);
}
