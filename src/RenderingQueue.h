#pragma once

#include <vector>
#include <memory>
#include "VertexBuffer.h"
#include "RenderPacket.h"
#include "UniformNode.h"
#include "UniformHelper.h"
#include "Pool.h"

class RenderingQueue
{
private:
    std::vector<RenderPacket> m_Packets;

    Pool<UniformMatrix4fNode> m_Mat4UniformPool;
    Pool<UniformIntegerNode> m_IntUniformPool;

    Pool<TextureNode> m_TexturePool;

    static void set_uniforms(ShaderProgram * active_shader, IUniformNode* first, TextureNode* first_texture);
    static void set_textures(TextureNode*);

    static void draw_vbo(VertexBuffer*, GLenum mode, std::size_t, std::size_t);
    static void draw_ibo(GLenum mode, std::size_t, std::size_t);
    static void setup_blending(ShaderProgram*, BlendingState&);

public:
    RenderingQueue() = default;

    IUniformNode* create_uniform(IUniformNode* prev, Uniform type, int value);
    IUniformNode* create_uniform(IUniformNode* prev, Uniform type, const Matrix4f& value);

    TextureNode* create_texture(TextureNode* prv, Texture * value, Uniform type);

    void push_render_packet(const RenderPacket&);

    void draw_all();
    void clear();
};
