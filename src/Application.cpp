#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Application.h"
#include "VertexLayout.h"
#include "Shaders.h"
#include "AttributeHelper.h"

int ReadFile(const char*, std::string&);

void Application::configure_shaders()
{
    ShaderFile vertex_shader;
    vertex_shader.name = "/Users/danm3/opengl/cmake/shaders/shader.vs";
    vertex_shader.type = GL_VERTEX_SHADER;

    ShaderFile frag_shader;
    frag_shader.name = "/Users/danm3/opengl/cmake/shaders/shader.fs";
    frag_shader.type = GL_FRAGMENT_SHADER;

    m_Shaders.add_file(vertex_shader);
    m_Shaders.add_file(frag_shader);
}

bool Application::initialize(const char* window_name, std::size_t width, std::size_t height)
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

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    m_VertexLayout = std::make_shared<VertexLayout>();
    m_VertexBuffer = std::make_shared<VertexBuffer>();

    m_VertexLayout->AddVertexAttribute(AttributeType::Position, 2);
    m_VertexLayout->AddVertexAttribute(AttributeType::Color, 3);

    float data[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.8f, -0.2f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    m_VertexBuffer->create(data, m_VertexLayout.get(), sizeof(data) / m_VertexLayout->size());

    m_VertexBuffer->bind();

    this->configure_shaders();
    m_Shaders.create_shaders();

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
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_VertexBuffer->bind();
    m_Shaders.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application* handler = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(handler->m_Window, GLFW_TRUE);
}
