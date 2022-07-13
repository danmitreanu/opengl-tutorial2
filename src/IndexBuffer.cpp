#include "IndexBuffer.h"

#include <iostream>
#include "OpenGL.h"

IndexBuffer::IndexBuffer(GLuint vao)
{
    m_Vao = vao;
    glGenBuffers(1, &m_Ibo);
}

IndexBuffer::~IndexBuffer()
{
    if (m_Ibo)
        glDeleteBuffers(1, &m_Ibo);
}

void IndexBuffer::create(const void* data, const std::size_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}