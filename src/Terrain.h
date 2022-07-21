#pragma once

#include <memory>

#include "Texture.h"
#include "HeightMap.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderPacket.h"
#include "TextureNode.h"
#include "UniformNode.h"

class Terrain
{
private:
    std::shared_ptr<HeightMap> m_HeightMap;
    std::shared_ptr<VertexLayout> m_VertexLayout;
    std::shared_ptr<VertexBuffer> m_Vbo;
    std::shared_ptr<IndexBuffer> m_Ibo;

public:
    Terrain();

    void load_heightmap(std::shared_ptr<HeightMap>);
    void generate();
    RenderPacket get_packet(ShaderProgram*, TextureNode*, IUniformNode*);
};
