#pragma once

#include <vector>
#include <memory>
#include "RenderPacket.h"
#include "UniformNode.h"
#include "UniformHelper.h"
#include "Pool.h"

class RenderingQueue
{
private:
    std::vector<RenderPacket> m_Packets;

    std::vector<std::shared_ptr<IUniformNode>> m_Uniforms;
    std::vector<std::shared_ptr<TextureNode>> m_Textures;

    static void set_uniforms(IUniformNode*, ShaderProgram*);
    static void set_textures(TextureNode*);

public:
    RenderingQueue() = default;

    UniformMatrix4fNode* create_uniform_matrix4f();
    TextureNode* create_texture();

    void push_render_packet(const RenderPacket&);

    void draw_all();
    void clear();
};