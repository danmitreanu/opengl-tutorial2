#pragma once

#include <iostream>
#include "OpenGL.h"
#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "UniformNode.h"
#include "TextureNode.h"

enum class BlendingFunc
{
    NONE = 0,

    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA
};

struct BlendingState
{
    bool enabled = false;

    BlendingFunc source_func;
    BlendingFunc dest_func;

    bool equals(const BlendingState&) const;
};

bool operator==(BlendingState const&, BlendingState const&);
bool operator!=(BlendingState const&, BlendingState const&);

struct RenderPacket
{
    int priority = 0;

    ShaderProgram* shader;
    VertexBuffer* vbo;
    IndexBuffer* ibo;
    BlendingState blend;
    GLenum topology;
    std::size_t primitive_start;
    std::size_t primitive_end;
    IUniformNode* uniforms = nullptr;
    TextureNode* textures = nullptr;

    static bool compare(RenderPacket const&, RenderPacket const&);
};

bool operator<(RenderPacket const&, RenderPacket const&);

static constexpr std::size_t get_topology_size(GLenum topology)
{
    switch (topology)
    {
        case GL_LINES:      return 2;
        case GL_TRIANGLES:  return 3;
    };
}

static constexpr GLenum get_blending_func(BlendingFunc func)
{
    switch (func)
    {
        case BlendingFunc::SRC_COLOR:                   return GL_SRC_COLOR;
        case BlendingFunc::ONE_MINUS_SRC_COLOR:         return GL_ONE_MINUS_SRC_COLOR;
        case BlendingFunc::DST_COLOR:                   return GL_DST_COLOR;
        case BlendingFunc::ONE_MINUS_DST_COLOR:         return GL_ONE_MINUS_DST_COLOR;
        case BlendingFunc::SRC_ALPHA:                   return GL_SRC_ALPHA;
        case BlendingFunc::ONE_MINUS_SRC_ALPHA:         return GL_ONE_MINUS_SRC_ALPHA;
        case BlendingFunc::DST_ALPHA:                   return GL_DST_ALPHA;
        case BlendingFunc::ONE_MINUS_DST_ALPHA:         return GL_ONE_MINUS_DST_ALPHA;
        case BlendingFunc::CONSTANT_COLOR:              return GL_CONSTANT_COLOR;
        case BlendingFunc::ONE_MINUS_CONSTANT_COLOR:    return GL_ONE_MINUS_CONSTANT_COLOR;
        case BlendingFunc::CONSTANT_ALPHA:              return GL_CONSTANT_ALPHA;
        case BlendingFunc::ONE_MINUS_CONSTANT_ALPHA:    return GL_ONE_MINUS_CONSTANT_ALPHA;

        default: return 0;
    };
}