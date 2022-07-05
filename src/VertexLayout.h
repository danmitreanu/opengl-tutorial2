#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

class VertexBuffer;

struct VertexAttribute
{
    VertexAttribute()
        : number_of_floats(0)
    {
    }
    const char* name;
    std::size_t number_of_floats;
};

// vertex layout.
class VertexLayout
{
    // this class describes the attributes you have in the data:
public:
    VertexLayout() = default;

    std::size_t size() const;
    //^ returns the size in bytes of a single vertex

    void AddVertexAttribute(const char* name, const std::size_t number_of_floats);

    const std::vector<VertexAttribute> get_attributes() const;

private:
    std::vector<VertexAttribute> m_Attributes;
};

// vertex buffer.
class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

public:
    void bind();

public:
    void create(const float* data, const VertexLayout& vertex_layout, const std::size_t vertex_count);
    //^ this function needs to bind the VAO, bind the buffer, send the data to the
    // gpu and assign offsets with glVertexAttribPointer

private:
    GLuint m_Vao;
    GLuint m_Buffer;
};