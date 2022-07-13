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

    std::array<int, (std::size_t)AttributeType::Count> m_AttribIndeces;

    friend class IndexBuffer;

public:
    VertexBuffer();
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
    void create(const void*, const VertexLayout*, const std::size_t);
};
