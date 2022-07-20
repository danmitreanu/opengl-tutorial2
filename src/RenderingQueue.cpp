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

    for (const auto& packet : m_Packets)
    {
        if (current_vbo != packet.vbo)
        {
            current_vbo = packet.vbo;
            current_vbo->bind();
        }

        if (bound_textures_hash != packet.textures->hash)
        {
            bound_textures_hash = packet.textures->hash;
            set_textures(packet.textures);
        }

        if (active_shader != packet.shader)
        {
            active_shader = packet.shader;
            active_shader->bind();
        }

        set_uniforms(active_shader, packet.uniforms, packet.textures);

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
            draw_vbo(packet.topology, packet.primitive_start, packet.primitive_end);
        }
    }

    current_vbo->unbind();
}

void RenderingQueue::draw_vbo(
    GLenum mode,
    std::size_t primitive_start,
    std::size_t primitive_end)
{
    std::size_t primitive_size = get_topology_size(mode);
    std::size_t primitive_count = primitive_end - primitive_start;
    std::size_t start = primitive_size * primitive_start;
    std::size_t count = primitive_size * primitive_count;

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

void RenderingQueue::set_uniforms(ShaderProgram* active_shader, IUniformNode* first, TextureNode* first_texture)
{
    {
        auto node = first;
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

    uint32_t slot = 0;
    while (node != nullptr && slot <= 8)
    {
        node->value->bind(slot);
        node->slot = slot;
        slot++;
        node = node->next;
    }
}
