#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "OpenGL.h"

#include "Application.h"
#include "ApplicationBase.h"
#include "VertexLayout.h"
#include "ShaderProgram.h"
#include "AttributeHelper.h"

void Application::init_buffer()
{
    m_VertexLayout = std::make_shared<VertexLayout>();
    m_VertexBuffer = std::make_shared<VertexBuffer>();
    m_IndexBuffer = std::make_shared<IndexBuffer>();

    m_VertexLayout->AddVertexAttribute(AttributeType::Position, 3);
    m_VertexLayout->AddVertexAttribute(AttributeType::Color, 3);
    m_VertexLayout->AddVertexAttribute(AttributeType::UV, 2);
    
    struct Vertex
    {
        Vector3f pos;
        Vector3f c;
        Vector2f uv;
    };
 
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
 
    for (std::size_t y = 0; y < 100; y++)
    {
        for (std::size_t x = 0; x < 100; x++)
        {
            Vertex v;
            v.pos = Vector3f(float(x), float(y), -100.0f);
            v.c = Vector3f(1.0f, 0.0f, 0.0f);
            v.uv = Vector2f(float(x) / 100.0f, float(y) / 100.0f);
            vertices.push_back(v);
        }
    }

    for (std::size_t y = 0; y < 99; y++)
    {
        for (std::size_t x = 0; x < 99; x++)
        {
            indices.push_back(y * 100 + x);
            indices.push_back(y * 100 + x + 1);
            indices.push_back((y + 1) * 100 + x);
            indices.push_back(y * 100 + x + 1);
            indices.push_back((y + 1) * 100 + x + 1);
            indices.push_back((y + 1) * 100 + x);
        }
    }

    m_VertexBuffer->create(vertices.data(), m_VertexLayout.get(), vertices.size());
    m_IndexBuffer->create(m_VertexBuffer.get(), indices.data(), indices.size());
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
}

void Application::init_camera()
{
    m_Camera.change_framebuff_dimensions(get_width(), get_height());

    Vector3f look_at{ 0.0, 0.0f, 0.0f };
 
    m_Camera.set(look_at);
}

void Application::init_texture()
{
    m_Texture = m_ResourceManager.get_texture("/Users/danm3/opengl/cmake/resources/mc.jpeg");
}

bool Application::initialize(const char* window_name, std::size_t width, std::size_t height)
{
    if (!init_glfw(window_name, width, height))
        return false;

    this->set_key_callback(Application::key_callback);
    this->set_framebuffer_callback(Application::framebuffer_size_callback);
 
    init_buffer();
    init_shader();
    init_camera();
    init_texture();

    return true;
}

void Application::update(const float delta_seconds)
{
    update_offset(delta_seconds);

    m_ModelMatrix.InitIdentity();
    m_ModelMatrix.InitTranslationTransform(m_Offset.x, m_Offset.y, 0.0f);
    
    m_Camera.update_camera_matrices();
}

void Application::render()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    m_VertexBuffer->bind();
    m_IndexBuffer->bind();
 
    m_Texture->bind(0);

    m_Shaders->bind();
    //m_Shaders->set_uniform(Uniform::Texture0, 0);
    m_Shaders->set_uniform(Uniform::MVP, m_Camera.get_mvp(m_ModelMatrix));
 
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, 58806, GL_UNSIGNED_INT, 0);

    m_VertexBuffer->unbind();
}

void Application::key_callback(void* app, int key, int action)
{
    Application* handler = reinterpret_cast<Application*>(app);

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

void Application::framebuffer_size_callback(void* app, std::size_t width, std::size_t height)
{
    Application* handler = reinterpret_cast<Application*>(app);
    handler->m_Camera.change_framebuff_dimensions(width, height);
}

void Application::key_down(int key)
{
    switch (key)
    {
        case GLFW_KEY_W: m_Movement.up = true; break;
        case GLFW_KEY_S: m_Movement.down = true; break;
        case GLFW_KEY_A: m_Movement.left = true; break;
        case GLFW_KEY_D: m_Movement.right = true; break;
        case GLFW_KEY_Q: m_Movement.yaw_left = true; break;
        case GLFW_KEY_E: m_Movement.yaw_right = true; break;
        case GLFW_KEY_Z: m_Movement.pitch_up = true; break;
        case GLFW_KEY_X: m_Movement.pitch_down = true; break;
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
        case GLFW_KEY_Q: m_Movement.yaw_left = false; break;
        case GLFW_KEY_E: m_Movement.yaw_right = false; break;
        case GLFW_KEY_Z: m_Movement.pitch_up = false; break;
        case GLFW_KEY_X: m_Movement.pitch_down = false; break;
    }
}

void Application::update_offset(float delta_seconds)
{
    const float speed = 150.0f; // units per sec
    const float camera_coeff = 0.5f;

    float diff = delta_seconds * speed;

    if (m_Movement.yaw_left != m_Movement.yaw_right)
    {
        float yaw_dir = -1.0f * int(m_Movement.yaw_left) + 1.0f * int(m_Movement.yaw_right);
        m_Camera.on_yaw(yaw_dir * diff * camera_coeff);
    }

    if (m_Movement.pitch_up != m_Movement.pitch_down)
    {
        float pitch_dir = -1.0f * int(m_Movement.pitch_up) + 1.0f * int(m_Movement.pitch_down);
        m_Camera.on_pitch(pitch_dir * diff * camera_coeff);
    }
 
    if (m_Movement.up != m_Movement.down)
    {
        float direction = -1.0f * int(m_Movement.up) + 1.0f * int(m_Movement.down);
        m_Camera.on_move_forward(direction * diff);
    }

    if (m_Movement.left != m_Movement.right)
    {
        float direction = 1.0f * int(m_Movement.left) - 1.0f * int(m_Movement.right);
        m_Camera.on_move_side(direction * diff);
    }
}
