#include "IndexBuffer.h"

#include <iostream>
#include "VertexBuffer.h"
#include "OpenGL.h"

IndexBuffer::IndexBuffer(const VertexBuffer* vertex_buffer)
{
    m_Vao = vertex_buffer->m_Vao;
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

void IndexBuffer::create(const void* data, const std::size_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}