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
#include "ApplicationBaseKeys.h"

#include "Movement.h"
#include "RenderingQueue.h"

class Engine : public ApplicationBase
{
private:
    Camera m_Camera;

    std::shared_ptr<VertexLayout> m_VertexLayout;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;

    RenderingQueue m_RenderQueue;

    std::shared_ptr<Texture> m_Texture;
    std::shared_ptr<Texture> m_Texture2;
    std::shared_ptr<HeightMap> m_HeightMap;
    std::shared_ptr<Terrain> m_Terrain;

    Matrix4f m_ModelMatrix;
    Movement m_Movement;
    Vector3f m_Offset{ 0, 0, 0 };

    void init_buffer();
    void init_camera();
    void init_texture();
    void init_terrain();

    void key_callback(ApplicationBase*, ApplicationBaseKey, ApplicationBaseKeyAction) override;
    void framebuffer_callback(ApplicationBase*, std::size_t, std::size_t) override;
    void focus_callback(ApplicationBase*, bool) override;
    void on_key(ApplicationBaseKey, bool);
    void update_movement(float delta, const Vector2f& mouse);

    // ApplicationBase implementations
    void update(const float delta_time);
    void render();

public:
    Engine() = default;
    Engine& operator=(const Engine&) = delete;

    bool initialize(const char* window_name, std::size_t width, std::size_t height);
};
