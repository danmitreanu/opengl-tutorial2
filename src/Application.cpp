#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Application.h"
#include "VertexLayout.h"
#include "Shaders.h"
#include "AttributeHelper.h"

bool Application::init_glfw(const char* window_name, std::size_t width, std::size_t height)
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(width, height, window_name, NULL, NULL);

    if (!m_Window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetKeyCallback(m_Window, Application::key_callback);
    glfwSetWindowUserPointer(m_Window, this);

    return true;
}

void Application::init_buffer(const void* data, std::size_t size)
{
    m_VertexLayout = std::make_shared<VertexLayout>();
    m_VertexBuffer = std::make_shared<VertexBuffer>();

    m_VertexLayout->AddVertexAttribute(AttributeType::Position, 2);
    m_VertexLayout->AddVertexAttribute(AttributeType::Color, 3);

    m_VertexBuffer->create(data, m_VertexLayout.get(), size / m_VertexLayout->size());
    m_VertexBuffer->bind();
}

void Application::init_shader()
{
    m_Shaders = std::make_shared<Shaders>();
    const char* vertex_shader_file = "/Users/danm3/opengl/cmake/shaders/shader.vs";
    const char* frag_shader_file = "/Users/danm3/opengl/cmake/shaders/shader.fs";

    m_Shaders->create(vertex_shader_file, frag_shader_file);
    m_Shaders->bind();
}

bool Application::initialize(const char* window_name, std::size_t width, std::size_t height)
{
    if (!init_glfw(window_name, width, height))
        return false;

    float data[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.8f, -0.2f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    init_buffer((void*)data, sizeof(data));
    init_shader();

    return true;
}

void Application::run()
{
    float delta_time = glfwGetTime();

    while (!glfwWindowShouldClose(m_Window))
    {
        float now_time = glfwGetTime();

        this->update(now_time - delta_time);
        this->render();
        delta_time = now_time;

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void Application::update(const float delta_seconds)
{
    //std::cout << delta_seconds << std::endl;
}

void Application::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
 
    m_VertexBuffer->bind();
    m_Shaders->bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_VertexBuffer->unbind();
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application* handler = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(handler->m_Window, GLFW_TRUE);
        return;
    }

    switch (action)
    {
        case GLFW_PRESS:
            handler->key_down(key);
            return;

        case GLFW_RELEASE:
            handler->key_up(key);
            return;k
    }
}

void Application::key_down(int key)
{
    switch (key)
    {
        case GLFW_KEY_W: m_Movement.up = true; break;
        case GLFW_KEY_S: m_Movement.down = true; break;
        case GLFW_KEY_A: m_Movement.left = true; break;
        case GLFW_KEY_D: m_Movement.right = true; break;
    }
}

void Application::key_up(int key)
{
    switch (key)
    {
        case GLFW_KEY_W: m_Movement.up = false; break;
        case GLFW_KEY_S: m_Movement.down = false; break;
        case GLFW_KEY_A: m_Movement.left = false; break;
        case GLFW_KEY_D: m_Movement.right = false; break;
    }
}