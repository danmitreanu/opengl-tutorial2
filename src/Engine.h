#pragma once

#include "OpenGL.h"
#include <iostream>
#include <memory>

#include "VertexLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Math3D.h"
#include "Texture.h"
#include "Terrain.h"
#include "ResourceManager.h"
#include "ApplicationBase.h"
#include "ApplicationBaseInputs.h"

#include "RenderingQueue.h"

class Engine : public ApplicationBase
{
private:
    bool m_Active = true;
    Camera m_Camera;

    RenderingQueue m_RenderQueue;
    ResourceManager m_ResourceManager;

    std::shared_ptr<Texture> m_Texture;
    std::shared_ptr<Texture> m_Texture2;
    std::shared_ptr<HeightMap> m_HeightMap;
    std::shared_ptr<Terrain> m_Terrain;

    Matrix4f m_ModelMatrix;
    Vector3f m_Offset{ 0, 0, 0 };

    void init_camera();
    void init_texture();
    void init_terrain();

    void on_key(ApplicationBaseKey, bool);
    void update_movement(InputState&, const float delta);

    // ApplicationBase implementations
    void update(const WindowState&, InputState&, const float);
    void render();

public:
    Engine() = default;
    Engine& operator=(const Engine&) = delete;

    bool initialize(const char* window_name, std::size_t width, std::size_t height);

    void before_run(const WindowState&, InputState&) override;
};
