#pragma once

#include <vector>
#include <memory>
#include "RenderPacket.h"
#include "UniformNode.h"
#include "UniformHelper.h"

class RenderingQueue
{
private:
    std::vector<RenderPacket> m_Packets;

    IUniformNode* m_CommonUniforms = nullptr;

    void set_common_uniforms(ShaderProgram*);
    static void set_uniforms(IUniformNode*, ShaderProgram*);
    static void set_textures(TextureNode*);

public:
    RenderingQueue() = default;

    void push_common_uniform(IUniformNode*);
    void push_render_packet(const RenderPacket&);

    void draw_all();
    void clear();
};