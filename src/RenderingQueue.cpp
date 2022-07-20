#include "RenderingQueue.h"

#include <vector>
#include <algorithm>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderPacket.h"
#include "UniformNode.h"

UniformMatrix4fNode* RenderingQueue::create_uniform_matrix4f()
{
    return m_Mat4UniformPool.alloc();
}

TextureNode* RenderingQueue::create_texture()
{
    return m_TexturePool.alloc();
}

void RenderingQueue::push_render_packet(const RenderPacket& packet)
{
    m_Packets.push_back(packet);
}

void RenderingQueue::clear()
{
    m_Packets.clear();
    m_Mat4UniformPool.clear();
    m_TexturePool.clear();
}

void RenderingQueue::draw_all()
{
    std::sort(m_Packets.begin(), m_Packets.end());

    VertexBuffer* current_vbo = nullptr;
    IndexBuffer* current_ibo = nullptr;
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

        packet.shader->bind();
        set_uniforms(packet.uniforms, packet.shader);

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

void RenderingQueue::set_uniforms(IUniformNode* first, ShaderProgram* shader)
{
    auto node = first;

    while (node != nullptr)
    {
        node->set_uniform(shader);

        node = node->next;
    }
}

void RenderingQueue::set_textures(TextureNode* first)
{
    auto node = first;

    std::size_t slot = 0;
    while (node != nullptr && slot <= 8)
    {
        node->value->bind(slot);

        slot++;
        node = node->next;
    }
}
