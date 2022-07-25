#include "OpenGL.h"
#include "Terrain.h"
#include "VertexLayout.h"
#include "RenderPacket.h"
#include "Math3D.h"
#include "ResourceManager.h"
#include "RenderingQueue.h"

Terrain::Terrain()
{
    m_VertexLayout = std::make_shared<VertexLayout>();
    m_Vbo = std::make_shared<VertexBuffer>();
    m_Ibo = std::make_shared<IndexBuffer>();

    m_VertexLayout->AddVertexAttribute(AttributeType::Position, 3);
    m_VertexLayout->AddVertexAttribute(AttributeType::UV, 2);
}

void Terrain::init_textures(ResourceManager* resource_manager)
{
    m_GrassTex = resource_manager->get_texture("high_grass.jpeg");
    m_Rock1Tex = resource_manager->get_texture("high_rock1.jpeg");
    m_Rock2Tex = resource_manager->get_texture("high_rock2.jpeg");
    m_SnowTex = resource_manager->get_texture("high_snow.jpeg");

    m_SplatMap = resource_manager->get_texture("splatmap.tga");
}

void Terrain::load_heightmap(std::shared_ptr<HeightMap> height_map)
{
    m_HeightMap = height_map;
    m_Width = m_HeightMap->get_width();
    m_Height = m_HeightMap->get_height();
}

void Terrain::generate()
{
    struct Vertex
    {
        Vector3f pos;
        Vector2f uv;
    };

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (std::size_t y = 0; y < m_Height; y++)
    for (std::size_t x = 0; x < m_Width; x++)
    {
        float th = m_HeightMap->get_height(x, y);

        Vertex v;
        v.pos = Vector3f{ x * m_SizeMultiplier, y * m_SizeMultiplier, th };
        v.uv = Vector2f{ float(x) / m_Width, float(y) / m_Height };
        vertices.push_back(v);
    }

    for (std::size_t y = 0; y < m_Height - 1; y++)
    for (std::size_t x = 0; x < m_Width - 1; x++)
    {
        indices.push_back(y * m_Width + x);
        indices.push_back(y * m_Width + x + 1);
        indices.push_back((y + 1) * m_Width + x);
        indices.push_back(y * m_Width + x + 1);
        indices.push_back((y + 1) * m_Width + x + 1);
        indices.push_back((y + 1) * m_Width + x);
    }

    m_Vbo->create(vertices.data(), m_VertexLayout.get(), vertices.size());
    m_Ibo->create(m_Vbo.get(), indices.data(), indices.size());
}

RenderPacket Terrain::get_packet(
    RenderingQueue* render_queue,
    ShaderProgram* shader,
    IUniformNode* uniforms)
{
    auto* tex = render_queue->create_texture(nullptr, m_GrassTex.get(), Uniform::Texture0);
    tex = render_queue->create_texture(tex, m_Rock1Tex.get(), Uniform::Texture1);
    tex = render_queue->create_texture(tex, m_Rock2Tex.get(), Uniform::Texture2);
    tex = render_queue->create_texture(tex, m_SnowTex.get(), Uniform::Texture3);
    tex = render_queue->create_texture(tex, m_SplatMap.get(), Uniform::Texture4);

    RenderPacket packet;
    packet.vbo = m_Vbo.get();
    packet.ibo = m_Ibo.get();
    packet.shader = shader;
    packet.topology = GL_TRIANGLES;
    packet.primitive_start = 0;
    packet.primitive_end = m_Ibo->get_count() / get_topology_size(GL_TRIANGLES);
    packet.textures = tex;
    packet.uniforms = uniforms;

    return packet;
}
