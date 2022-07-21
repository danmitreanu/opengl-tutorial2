#include "OpenGL.h"
#include "Terrain.h"
#include "VertexLayout.h"
#include "RenderPacket.h"
#include "Math3D.h"

Terrain::Terrain()
{
    m_VertexLayout = std::make_shared<VertexLayout>();
    m_Vbo = std::make_shared<VertexBuffer>();
    m_Ibo = std::make_shared<IndexBuffer>();

    m_VertexLayout->AddVertexAttribute(AttributeType::Position, 3);
    m_VertexLayout->AddVertexAttribute(AttributeType::UV, 2);
    m_VertexLayout->AddVertexAttribute(AttributeType::Height, 1);
}

void Terrain::load_heightmap(std::shared_ptr<HeightMap> height_map)
{
    m_HeightMap = height_map;
}

void Terrain::generate()
{
    struct Vertex
    {
        Vector3f pos;
        Vector2f uv;
        float height;
    };

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    std::size_t height = m_HeightMap->get_height();
    std::size_t width = m_HeightMap->get_width();
    float size_multiplier = 2.0f;
    std::size_t tex_size = 20;

    for (std::size_t y = 0; y < height; y++)
    for (std::size_t x = 0; x < width; x++)
    {
        float height = m_HeightMap->get_height(x, y) * 1000.0f - 800.0f;

        Vertex v;
        v.pos = Vector3f{ x * size_multiplier, y * size_multiplier, height };
        v.uv = Vector2f{ float(x % tex_size) / tex_size, float(y % tex_size) / tex_size };
        v.height = height;
        vertices.push_back(v);
    }

    for (std::size_t y = 0; y < height - 1; y++)
    for (std::size_t x = 0; x < width - 1; x++)
    {
        indices.push_back(y * width + x);
        indices.push_back(y * width + x + 1);
        indices.push_back((y + 1) * width + x);
        indices.push_back(y * width + x + 1);
        indices.push_back((y + 1) * width + x + 1);
        indices.push_back((y + 1) * width + x);
    }

    m_Vbo->create(vertices.data(), m_VertexLayout.get(), vertices.size());
    m_Ibo->create(m_Vbo.get(), indices.data(), indices.size());
}

RenderPacket Terrain::get_packet(ShaderProgram* shader, TextureNode* textures, IUniformNode* uniforms)
{
    RenderPacket packet;
    packet.vbo = m_Vbo.get();
    packet.ibo = m_Ibo.get();
    packet.shader = shader;
    packet.topology = GL_TRIANGLES;
    packet.primitive_start = 0;
    packet.primitive_end = m_Ibo->get_count() / get_topology_size(GL_TRIANGLES);
    packet.textures = textures;
    packet.uniforms = uniforms;

    return packet;
}
