#pragma once

class IndexBuffer
{
private:
    GLuint m_Ibo;
    GLuint m_Vao;

public:
    IndexBuffer(GLuint vao);
    ~IndexBuffer();

    void bind();
    void create(const void*, const std::size_t);
};