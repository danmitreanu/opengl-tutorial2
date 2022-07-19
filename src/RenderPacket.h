#pragma once

#include <iostream>
#include "OpenGL.h"
#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "UniformNode.h"
#include "TextureNode.h"

struct RenderPacket
{
    ShaderProgram* shader;
    VertexBuffer* vbo;
    IndexBuffer* ibo;
    GLenum topology;
    std::size_t primitive_start;
    std::size_t primitive_end;
    IUniformNode* uniforms = nullptr;
    TextureNode* textures = nullptr;
};

static std::size_t get_topology_size(GLenum topology)
{
    switch (topology)
    {
        case GL_LINES:      return 2;
        case GL_TRIANGLES:  return 3;
    };
}