#include <iostream>
#include <fstream>
#include <sstream>

#include "OpenGL.h"

#include "Application.h"
#include "VertexLayout.h"
#include "ShaderProgram.h"
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

#ifdef _WIN32
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW could not be initialized." << std::endl;
        return false;
    }
#endif

    return true;
}

void Application::init_buffer(const void* data, std::size_t size)
{
    m_VertexLayout = std::make_shared<VertexLayout>();
    m_VertexBuffer = std::make_shared<VertexBuffer>();

    m_VertexLayout->AddVertexAttribute(AttributeType::Position, 3);
    m_VertexLayout->AddVertexAttribute(AttributeType::Color, 3);

    m_VertexBuffer->create(data, m_VertexLayout.get(), size / m_VertexLayout->size());
    m_VertexBuffer->bind();
}

void Application::init_shader()
{
    m_Shaders = std::make_shared<ShaderProgram>();
#ifndef _WIN32
    const char* vertex_shader_file = "/Users/danm3/opengl/cmake/shaders/shader.vs";
    const char* frag_shader_file = "/Users/danm3/opengl/cmake/shaders/shader.fs";
#else
    const char* vertex_shader_file = "X:\\opengl-tutorial2\\shaders\\shader.vs";
    const char* frag_shader_file = "X:\\opengl-tutorial2\\shaders\\shader.fs";
#endif

    m_Shaders->create(vertex_shader_file, frag_shader_file);
    m_Shaders->bind();
}

bool Application::initialize(const char* window_name, std::size_t width, std::size_t height)
{
    if (!init_glfw(window_name, width, height))
        return false;

    float data[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.8f, -0.2f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    init_buffer((void*)data, sizeof(data));
    init_shader();

    m_VertexBuffer->bind_attributes(m_Shaders->get_program_id());

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
    update_offset(delta_seconds);

    m_Shaders->set_uniform(Uniform::Offset, m_Offset);
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
            return;
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

void Application::update_offset(float delta_seconds)
{
    const float speed = 0.4; // coords per sec

    float diff = delta_seconds * speed;

    if (m_Movement.left)
        m_Offset.x -= diff;

    if (m_Movement.right)
        m_Offset.x += diff;

    if (m_Movement.up)
        m_Offset.y += diff;

    if (m_Movement.down)
        m_Offset.y -= diff;
}
