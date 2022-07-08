#pragma once

#include "OpenGL.h"
#include <iostream>
#include <memory>

#include "VertexLayout.h"
#include "VertexBuffer.h"
#include "Shaders.h"

#include "Movement.h"

class Application
{
public:
    Application() = default;
    Application& operator=(const Application&) = delete;

    bool initialize(const char* window_name, std::size_t width, std::size_t height);
    void update(const float delta_seconds);
    void render();
    void run();

private:
    GLFWwindow* m_Window = nullptr;

    std::shared_ptr<VertexLayout> m_VertexLayout;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;

    std::shared_ptr<Shaders> m_Shaders;

    Movement m_Movement;

    bool init_glfw(const char*, std::size_t, std::size_t);
    void init_buffer(const void*, std::size_t);
    void init_shader();

    static void key_callback(GLFWwindow*, int, int, int, int);
    void key_down(int);
    void key_up(int);
};
