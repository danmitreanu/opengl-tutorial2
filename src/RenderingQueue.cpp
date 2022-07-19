#include "RenderingQueue.h"

#include <vector>
#include "RenderPacket.h"
#include "UniformNode.h"

UniformMatrix4fNode* RenderingQueue::create_uniform_matrix4f()
{
    auto ptr = std::make_shared<UniformMatrix4fNode>();
    m_Uniforms.push_back(ptr);

    return ptr.get();
}

TextureNode* RenderingQueue::create_texture()
{
    auto ptr = std::make_shared<TextureNode>();
    m_Textures.push_back(ptr);

    return ptr.get();
}

void RenderingQueue::push_render_packet(const RenderPacket& packet)
{
    m_Packets.push_back(packet);
}

void RenderingQueue::clear()
{
    m_Packets.clear();
    m_Uniforms.clear();
    m_Textures.clear();
}

void RenderingQueue::draw_all()
{
    for (const auto& packet : m_Packets)
    {
        packet.vbo->bind();
        packet.ibo->bind();

        set_textures(packet.textures);

        packet.shader->bind();
        set_uniforms(packet.uniforms, packet.shader);

        std::size_t primitive_size = get_topology_size(packet.topology);
        std::size_t primitive_count = packet.primitive_end - packet.primitive_start;

        std::size_t ib_start = primitive_size * packet.primitive_start;
        std::size_t ib_count = primitive_size * primitive_count;

        glDrawElements(packet.topology, ib_count, GL_UNSIGNED_INT, (void*)ib_start);

        packet.vbo->unbind();
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
        node->value->bind(slot);

        slot++;
        node = node->next;
    }
}
