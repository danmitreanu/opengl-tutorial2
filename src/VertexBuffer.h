#pragma once

#include <iostream>
#include <OpenGL/gl3.h>

class VertexLayout;

class VertexBuffer
{
private:
    GLuint m_Vao;
    GLuint m_Buffer;

public:
    VertexBuffer();
    ~VertexBuffer();

    void bind();
    void create(const float*, const VertexLayout*, const std::size_t);
};