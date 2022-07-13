#pragma once

#include "OpenGL.h"
#include "VertexBuffer.h"

class IndexBuffer
{
private:
    GLuint m_Ibo;
    GLuint m_Vao;

public:
    IndexBuffer();
    ~IndexBuffer();

    void bind();
    void create(const VertexBuffer*, const void*, const std::size_t);
};
