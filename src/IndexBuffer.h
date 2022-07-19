#pragma once

#include "OpenGL.h"
#include "VertexBuffer.h"

class IndexBuffer
{
private:
    GLuint m_Ibo;
    std::size_t m_IndexCount = 0;

public:
    IndexBuffer();
    ~IndexBuffer();

    inline std::size_t get_count() { return m_IndexCount; }

    void bind();
    void create(const VertexBuffer*, const uint32_t* data, const std::size_t index_count);
};
