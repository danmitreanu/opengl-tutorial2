#pragma once

#include "OpenGL.h"
#include <iostream>
#include <memory>

#include "VertexLayout.h"
#include "VertexBuffer.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Math3D.h"

#include "Movement.h"

class Application
{
public:
    Application() = default;
    Application& operator=(const Application&) = delete;

    bool initialize(const char* window_name, std::size_t width, std::size_t height);
    void update(const float delta_seconds);
    void render();

    // Main loop
    void run();

private:
    GLFWwindow* m_Window = nullptr;
    // Framebuffer size:
    std::size_t m_Width;
    std::size_t m_Height;

    Camera m_Camera;

    std::shared_ptr<VertexLayout> m_VertexLayout;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;

    std::shared_ptr<ShaderProgram> m_Shaders;

    Matrix4f m_ModelMatrix;
    Movement m_Movement;
    Vector3f m_Offset{ 0, 0, 0 };

    bool init_glfw(const char*, std::size_t, std::size_t);
    void init_buffer(const void*, std::size_t);
    void init_shader();
    void init_camera();

    static void key_callback(GLFWwindow*, int, int, int, int);
    static void framebuffer_size_callback(GLFWwindow*, int, int);
    void key_down(int);
    void key_up(int);
    void update_offset(float);
};
