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

class Terrain
{
private:
    std::shared_ptr<HeightMap> m_HeightMap;
    std::shared_ptr<VertexLayout> m_VertexLayout;
    std::shared_ptr<VertexBuffer> m_Vbo;
    std::shared_ptr<IndexBuffer> m_Ibo;

    std::shared_ptr<Texture> m_GrassTex;
    std::shared_ptr<Texture> m_Rock1Tex;
    std::shared_ptr<Texture> m_Rock2Tex;
    std::shared_ptr<Texture> m_SnowTex;

public:
    Terrain();

    void init_textures(ResourceManager*);
    void load_heightmap(std::shared_ptr<HeightMap>);
    void generate();
    RenderPacket get_packet(RenderingQueue*, ShaderProgram*, IUniformNode*);
};
