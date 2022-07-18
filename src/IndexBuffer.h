#pragma once

#include "OpenGL.h"
#include "VertexBuffer.h"

class IndexBuffer
{
private:
    GLuint m_Ibo;

public:
    IndexBuffer();
    ~IndexBuffer();

    void bind();
    void create(const VertexBuffer*, const uint32_t* data, const std::size_t index_count);
};
