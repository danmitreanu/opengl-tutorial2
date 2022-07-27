#pragma once

#include <iostream>
#include <array>
#include "AttributeHelper.h"
#include "OpenGL.h"

class VertexLayout;

class VertexBuffer
{
private:
    GLuint m_Vao;
    GLuint m_Buffer;

    std::size_t m_VertexCount = 0;
    std::size_t m_VertexSize = 0;

    std::array<int, (std::size_t)AttributeType::Count> m_AttribIndeces;

    friend class IndexBuffer;

public:
    VertexBuffer();
    ~VertexBuffer();

    inline std::size_t get_vertex_count() const { return m_VertexCount; }
    inline std::size_t get_vertex_size() const { return m_VertexSize; }

    void bind() const;
    void unbind() const;
    void create(const void*, const VertexLayout*, const std::size_t);
};
