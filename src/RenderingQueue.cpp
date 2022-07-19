#include "RenderingQueue.h"

#include <vector>
#include "RenderPacket.h"
#include "UniformNode.h"

void RenderingQueue::push_common_uniform(IUniformNode* uniform_node)
{
    if (m_CommonUniforms == nullptr)
    {
        m_CommonUniforms = uniform_node;
        return;
    }

    auto node = m_CommonUniforms;
    while (node->next != nullptr)
        node = node->next;

    node->next = uniform_node;
}

void RenderingQueue::push_render_packet(const RenderPacket& packet)
{
    m_Packets.push_back(packet);
}

void RenderingQueue::clear()
{
    m_Packets.clear();
    m_CommonUniforms = nullptr;
}

void RenderingQueue::draw_all()
{
    for (const auto& packet : m_Packets)
    {
        packet.vbo->bind();
        packet.ibo->bind();
        // set uniforms
        // set textures

        set_textures(packet.textures);

        packet.shader->bind();
        set_common_uniforms(packet.shader);
        set_uniforms(packet.uniforms, packet.shader);

        std::size_t primitive_size = get_topology_size(packet.topology);
        std::size_t primitive_count = packet.primitive_end - packet.primitive_start;

        std::size_t ib_start = primitive_size * packet.primitive_start;
        std::size_t ib_count = primitive_size * primitive_count;

        glDrawElements(packet.topology, ib_count, GL_UNSIGNED_INT, (void*)ib_start);

        packet.vbo->unbind();
    }
}

void RenderingQueue::set_common_uniforms(ShaderProgram* shader)
{
    auto node = m_CommonUniforms;
    while (node != nullptr)
    {
        node->set_uniform(shader);
        node = node->next;
    }
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
        node->value.bind(slot);

        slot++;
        node = node->next;
    }
}
