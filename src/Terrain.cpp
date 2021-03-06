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
    m_VertexLayout->AddVertexAttribute(AttributeType::Normal, 3);

    m_WaterVertexLayout = std::make_shared<VertexLayout>();
    m_WaterVbo = std::make_shared<VertexBuffer>();
    m_WaterIbo = std::make_shared<IndexBuffer>();

    m_WaterVertexLayout->AddVertexAttribute(AttributeType::Position, 2);
}

void Terrain::init_resources(ResourceManager* resource_manager)
{
    m_GrassTex = resource_manager->get_texture("high_grass.jpeg");
    m_Rock1Tex = resource_manager->get_texture("high_rock1.jpeg");
    m_Rock2Tex = resource_manager->get_texture("high_rock2.jpeg");
    m_SnowTex = resource_manager->get_texture("high_snow.jpeg");

    m_SplatMap = resource_manager->get_texture("splatmap.tga");

    m_TerrainShader = resource_manager->get_shader("terrain_shader");
    m_WaterShader = resource_manager->get_shader("water_shader");

    BlendingState water_blending;
    water_blending.enabled = true;
    water_blending.source_func = BlendingFunc::SRC_ALPHA;
    water_blending.dest_func = BlendingFunc::ONE_MINUS_SRC_ALPHA;

    m_WaterShader->set_blending_state(water_blending);
}

void Terrain::load_heightmap(std::shared_ptr<HeightMap> height_map)
{
    m_HeightMap = height_map;
    m_Width = m_HeightMap->get_tex_width();
    m_Height = m_HeightMap->get_tex_height();
}

Vector3f Terrain::get_terrain_normal(std::size_t x, std::size_t y)
{
    static const Vector3f off{ 1.0f, 1.0f, 0.0f };
    float hL = m_HeightMap->get_height(x - off.x, y);
    float hR = m_HeightMap->get_height(x + off.x, y);
    float hD = m_HeightMap->get_height(x, y - off.y);
    float hU = m_HeightMap->get_height(x, y + off.y);

    Vector3f normal;
    normal.x = hL - hR;
    normal.y = hD - hU;
    normal.z = 0.01f;
    normal = normal.Normalize();

    return normal;
}

void Terrain::generate()
{
    generate_terrain();
    generate_water();
}

void Terrain::generate_terrain()
{
    struct Vertex
    {
        Vector3f pos;
        Vector2f uv;
        Vector3f normal;
    };

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (std::size_t y = 0; y < m_Height; y++)
    for (std::size_t x = 0; x < m_Width; x++)
    {
        float map_height = m_HeightMap->get_height(x, y);

        Vertex v;
        v.pos = Vector3f{ x * m_SizeMultiplier, y * m_SizeMultiplier, map_height };
        v.uv = Vector2f{ float(x) / m_Width, float(y) / m_Height };
        v.normal = get_terrain_normal(x, y);
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

void Terrain::generate_water()
{
    float width = m_Width * m_SizeMultiplier;
    float height = m_Height * m_SizeMultiplier;

    float vertices[] = {
        0.0f, 0.0f,
        0.0f, (float)height,
        (float)width, 0.0f,
        (float)width, (float)height
    };

    uint32_t indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    m_WaterVbo->create(vertices, m_WaterVertexLayout.get(), 4);
    m_WaterIbo->create(m_WaterVbo.get(), indices, 6);
}

RenderPacket Terrain::create_terrain_packet(
    RenderingQueue* render_queue,
    IUniformNode* uniforms)
{
    static constexpr std::size_t topology_size = get_topology_size(GL_TRIANGLES);

    auto* tex = render_queue->create_texture(nullptr, m_GrassTex.get(), Uniform::Texture0);
    tex = render_queue->create_texture(tex, m_Rock1Tex.get(), Uniform::Texture1);
    tex = render_queue->create_texture(tex, m_Rock2Tex.get(), Uniform::Texture2);
    tex = render_queue->create_texture(tex, m_SnowTex.get(), Uniform::Texture3);
    tex = render_queue->create_texture(tex, m_SplatMap.get(), Uniform::SplatMapTexture);

    RenderPacket packet;
    packet.vbo = m_Vbo.get();
    packet.ibo = m_Ibo.get();
    packet.shader = m_TerrainShader.get();
    packet.topology = GL_TRIANGLES;
    packet.primitive_start = 0;
    packet.primitive_end = m_Ibo->get_count() / topology_size;
    packet.textures = tex;
    packet.uniforms = uniforms;

    return packet;
}

RenderPacket Terrain::create_water_packet(
    RenderingQueue* render_queue,
    IUniformNode* uniforms)
{
    static constexpr std::size_t topology_size = get_topology_size(GL_TRIANGLES);

    RenderPacket packet;
    packet.vbo = m_WaterVbo.get();
    packet.ibo = m_WaterIbo.get();
    packet.shader = m_WaterShader.get();
    packet.topology = GL_TRIANGLES;
    packet.primitive_start = 0;
    packet.primitive_end = m_WaterIbo->get_count() / topology_size;
    packet.textures = nullptr;
    packet.uniforms = uniforms;

    return packet;
}

std::vector<RenderPacket> Terrain::get_packets(RenderingQueue* render_queue, IUniformNode* uniforms)
{
    std::vector<RenderPacket> packets;

    auto terrain_packet = create_terrain_packet(render_queue, uniforms);
    auto water_packet = create_water_packet(render_queue, uniforms);

    packets.push_back(terrain_packet);
    packets.push_back(water_packet);
    return packets;
}
