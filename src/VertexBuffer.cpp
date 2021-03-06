#include "OpenGL.h"

#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "VertexAttribute.h"

VertexBuffer::VertexBuffer()
{
    // Generate the buffer
    glGenVertexArrays(1, &m_Vao);
    glGenBuffers(1, &m_Buffer);
}

VertexBuffer::~VertexBuffer()
{
    if (m_Vao)
        glDeleteVertexArrays(1, &m_Vao);

    if (m_Buffer)
        glDeleteBuffers(1, &m_Buffer);
}

void VertexBuffer::bind() const
{
    glBindVertexArray(m_Vao);
}

void VertexBuffer::unbind() const
{
    glBindVertexArray(0);
}

void VertexBuffer::create(
    const void* data,
    const VertexLayout* vertex_layout,
    const std::size_t vertex_count)
{
    m_VertexCount = vertex_count;
    m_VertexSize = vertex_layout->size();

    auto& attrs = vertex_layout->get_attributes();

    int attr_count = attrs.size();

    // Bind the VAO and buffer and populate
    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_layout->size() * vertex_count, (void*)data, GL_STATIC_DRAW);

    std::size_t offset = 0;
    int index = 0;
    for (auto& attr : attrs)
    {
        glEnableVertexAttribArray((int)attr.type);
        glVertexAttribPointer((int)attr.type, attr.number_of_floats, GL_FLOAT, GL_FALSE, vertex_layout->size(), (void*)offset);

        m_AttribIndeces[(std::size_t)attr.type] = index;

        offset += attr.number_of_floats * sizeof(float);
        index++;
    }
 
    // Unbind VAO and buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


