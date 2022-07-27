#pragma once

#include <memory>

#include "Texture.h"
#include "HeightMap.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderPacket.h"
#include "TextureNode.h"
#include "UniformNode.h"
#include "ResourceManager.h"
#include "RenderingQueue.h"
#include "ShaderProgram.h"

class Terrain
{
private:
    std::shared_ptr<HeightMap> m_HeightMap;

    std::shared_ptr<VertexLayout> m_VertexLayout;
    std::shared_ptr<VertexBuffer> m_Vbo;
    std::shared_ptr<IndexBuffer> m_Ibo;

    std::shared_ptr<VertexLayout> m_WaterVertexLayout;
    std::shared_ptr<VertexBuffer> m_WaterVbo;
    std::shared_ptr<IndexBuffer> m_WaterIbo;

    std::shared_ptr<Texture> m_GrassTex;
    std::shared_ptr<Texture> m_Rock1Tex;
    std::shared_ptr<Texture> m_Rock2Tex;
    std::shared_ptr<Texture> m_SnowTex;

    std::shared_ptr<Texture> m_SplatMap;

    std::shared_ptr<ShaderProgram> m_TerrainShader;
    std::shared_ptr<ShaderProgram> m_WaterShader;

    std::size_t m_Width;
    std::size_t m_Height;
    float m_SizeMultiplier = 10.0f;

    void generate_terrain();
    void generate_water();

    RenderPacket create_terrain_packet(RenderingQueue*, IUniformNode*);
    RenderPacket create_water_packet(RenderingQueue*, IUniformNode*);

    Vector3f get_terrain_normal(std::size_t, std::size_t);

public:
    Terrain();

    inline float get_width() { return m_Width * m_SizeMultiplier; }
    inline float get_height() { return m_Height * m_SizeMultiplier; }

    void init_textures(ResourceManager*);
    void load_heightmap(std::shared_ptr<HeightMap>);
    void generate();
    std::vector<RenderPacket> get_packets(RenderingQueue*, IUniformNode*);
};
