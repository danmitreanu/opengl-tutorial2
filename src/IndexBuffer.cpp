#include "IndexBuffer.h"

#include <iostream>
#include "VertexBuffer.h"
#include "OpenGL.h"

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_Ibo);
}

IndexBuffer::~IndexBuffer()
{
    if (m_Ibo)
        glDeleteBuffers(1, &m_Ibo);
}

void IndexBuffer::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
}

void IndexBuffer::create(const VertexBuffer* vertex_buffer,
    const uint32_t* data,
    const std::size_t index_count)
{
    vertex_buffer->bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(uint32_t), data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_IndexCount = index_count;
}
