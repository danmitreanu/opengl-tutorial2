#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "VertexLayout.h"
#include "VertexBuffer.h"
#include "Shaders.h"

class Application
{
public:
    Application() = default;
    Application& operator=(const Application&) = delete;

    bool initialize(const char* window_name, std::size_t width, std::size_t height);
    void update(const float delta_seconds);
    void render();
    void run();

    Shaders* get_shaders();

private:
    GLFWwindow* m_Window;

    std::shared_ptr<VertexLayout> m_VertexLayout;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;

    Shaders m_Shaders;

    void configure_shaders();

    static void key_callback(GLFWwindow*, int, int, int, int);
};
