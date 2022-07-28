#include "RenderingQueue.h"

#include <vector>
#include <algorithm>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderPacket.h"
#include "UniformNode.h"

IUniformNode* RenderingQueue::create_uniform(IUniformNode* prev, Uniform type, const Matrix4f& value)
{
    auto* n = m_Mat4UniformPool.alloc();
    n->uniform = type;
    n->value = value;
    n->next = prev;
    return n;
}
IUniformNode* RenderingQueue::create_uniform(IUniformNode* prev, Uniform type, int value)
{
    auto* n = m_IntUniformPool.alloc();
    n->uniform = type;
    n->value = value;
    n->next = prev;
    return n;
}

TextureNode* RenderingQueue::create_texture(TextureNode* prv, Texture * value, Uniform type)
{
    auto* n = m_TexturePool.alloc();
    n->value = value;
    n->target = type;

    if (prv == nullptr)
        n->hash = value->get_hash();
    else
        n->hash = value->get_hash() ^ prv->hash;

    n->next = prv;
    return n;
}

void RenderingQueue::push_render_packet(const RenderPacket& packet)
{
    m_Packets.push_back(packet);
}

void RenderingQueue::clear()
{
    m_Packets.clear();
    m_Mat4UniformPool.clear();
    m_IntUniformPool.clear();
    m_TexturePool.clear();
}

void RenderingQueue::draw_all()
{
    std::sort(m_Packets.begin(), m_Packets.end());

    VertexBuffer* current_vbo = nullptr;
    IndexBuffer* current_ibo = nullptr;
    ShaderProgram* active_shader = nullptr;
    uint64_t bound_textures_hash = 0;
    BlendingState active_blend_state;

    for (const auto& packet : m_Packets)
    {
        if (current_vbo != packet.vbo)
        {
            current_vbo = packet.vbo;
            current_vbo->bind();
        }

        if (packet.textures != nullptr && bound_textures_hash != packet.textures->hash)
        {
            bound_textures_hash = packet.textures->hash;
            set_textures(packet.textures);
        }

        if (active_shader != packet.shader)
        {
            active_shader = packet.shader;
            active_shader->bind();
        }

        if (packet.uniforms != nullptr)
            set_uniforms(active_shader, packet.uniforms, packet.textures);

        setup_blending(packet.shader, active_blend_state);

        if (packet.ibo != nullptr)
        {
            if (current_ibo != packet.ibo)
            {
                current_ibo = packet.ibo;
                current_ibo->bind();
            }

            draw_ibo(packet.topology, packet.primitive_start, packet.primitive_end);
        }
        else
        {
            draw_vbo(packet.vbo, packet.topology, packet.primitive_start, packet.primitive_end);
        }
    }

    current_vbo->unbind();
}

void RenderingQueue::draw_vbo(
    VertexBuffer* vbo,
    GLenum mode,
    std::size_t primitive_start,
    std::size_t primitive_end)
{
    std::size_t element_count = vbo->get_vertex_count();
    std::size_t element_size = vbo->get_vertex_size();
    std::size_t topology_size = get_topology_size(mode);
    const std::size_t start = primitive_start * topology_size * element_size;
    const std::size_t count = (primitive_end - primitive_start) * topology_size;

    glDrawArrays(mode, start, count);
}

void RenderingQueue::draw_ibo(
    GLenum mode,
    std::size_t primitive_start,
    std::size_t primitive_end)
{
    std::size_t primitive_size = get_topology_size(mode);
    std::size_t primitive_count = primitive_end - primitive_start;
    std::size_t start = primitive_size * primitive_start;
    std::size_t count = primitive_size * primitive_count;

    glDrawElements(mode, count, GL_UNSIGNED_INT, (void*)start);
}

void RenderingQueue::setup_blending(ShaderProgram* shader_program, BlendingState& active_blending_state)
{
    auto disable_blending = [&]()
    {
        active_blending_state.enabled = false;
        glDisable(GL_BLEND);
    };

    if (shader_program == nullptr)
    {
        disable_blending();
        return;
    }

    auto& blend = shader_program->get_blending_state();
    if (blend == active_blending_state)
        return;

    if (!blend.enabled)
    {
        disable_blending();
        return;
    }

    glEnable(GL_BLEND);
    auto srcfunc = get_blending_func(blend.source_func);
    auto destfunc = get_blending_func(blend.dest_func);
    glBlendFunc(srcfunc, destfunc);

    active_blending_state = blend;
}

void RenderingQueue::set_uniforms(ShaderProgram* active_shader, IUniformNode* first_uniform, TextureNode* first_texture)
{
    {
        auto node = first_uniform;
        while (node != nullptr)
        {
            node->set_uniform(active_shader);

            node = node->next;
        }
    }
    {
        auto node = first_texture;
        std::size_t slot = 0;
        while (node != nullptr && slot <= 8)
        {
            active_shader->set_uniform(node->target, int(node->slot));
            node = node->next;
        }
    }
}

void RenderingQueue::set_textures(TextureNode* first)
{
    auto node = first;

    uint8_t slot = 0;
    while (node != nullptr && slot <= 8)
    {
        node->value->bind(slot);
        node->slot = slot;
        slot++;
        node = node->next;
    }
}
