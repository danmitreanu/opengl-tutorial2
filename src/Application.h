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
#include "ResourceManager.h"
#include "ApplicationBase.h"

#include "Movement.h"

class Application : public ApplicationBase
{
private:
    Camera m_Camera;

    std::shared_ptr<VertexLayout> m_VertexLayout;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;

    std::shared_ptr<ShaderProgram> m_Shaders;

    ResourceManager m_ResourceManager;
    std::shared_ptr<Texture> m_Texture;

    Matrix4f m_ModelMatrix;
    Movement m_Movement;
    Vector3f m_Offset{ 0, 0, 0 };

    void init_buffer();
    void init_shader();
    void init_camera();
    void init_texture();

    static void key_callback(void*, int, int);
    static void framebuffer_size_callback(void*, std::size_t, std::size_t);
    void key_down(int);
    void key_up(int);
    void update_offset(float);

    // ApplicationBase implementations
    void update(const float delta_time);
    void render();

public:
    Application() = default;
    Application& operator=(const Application&) = delete;

    bool initialize(const char* window_name, std::size_t width, std::size_t height);
};
