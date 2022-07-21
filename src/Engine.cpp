#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cmath>

#include "OpenGL.h"

#include "Engine.h"
#include "ApplicationBase.h"
#include "VertexLayout.h"
#include "ShaderProgram.h"
#include "AttributeHelper.h"
#include "RenderPacket.h"
#include "RenderingQueue.h"
#include "UniformNode.h"
#include "TextureNode.h"

void Engine::init_buffer()
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
            v.pos = Vector3f(float(x) * 3, float(y) * 3, 0.2 * (rand() % 50) - 100.0f);
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

void Engine::init_shader()
{
    m_Shaders = get_resource_manager().get_shader("shader");
}

void Engine::init_camera()
{
    m_Camera.change_framebuff_dimensions(get_width(), get_height());

    Vector3f look_at{ 0.0f, -50.0f, 0.0f };
 
    m_Camera.set(look_at);
}

void Engine::init_texture()
{
    m_Texture = get_resource_manager().get_texture("mc.jpeg");
    m_Texture2 = get_resource_manager().get_texture("grass.jpeg");
}

bool Engine::initialize(const char* window_name, std::size_t width, std::size_t height)
{
    if (!init_window(window_name, width, height))
        return false;

    init_buffer();
    init_shader();
    init_camera();
    init_texture();

    return true;
}

void Engine::update(const float delta_seconds)
{
    update_offset(delta_seconds);

    m_ModelMatrix.InitRotateTransform(0.0f, 30.0f, 0.0f);

    m_Camera.update_camera_matrices();
}

void Engine::render()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.4f, 0.6f, 0.7f, 0.0f);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*m_VertexBuffer->bind();
    m_IndexBuffer->bind();
 
    m_Texture->bind(0);

    m_Shaders->bind();
    //m_Shaders->set_uniform(Uniform::Texture0, 0);
    m_Shaders->set_uniform(Uniform::Model, m_ModelMatrix);
    m_Shaders->set_uniform(Uniform::MVP, m_Camera.get_vp());
 
    glDrawElements(GL_TRIANGLES, 58806, GL_UNSIGNED_INT, 0);

    m_VertexBuffer->unbind();*/

    RenderPacket packet;
    Matrix4f model_matrix;
    model_matrix.InitTranslationTransform(Vector3f{ 0.0f, 0.0f, 50.0f });

    auto* u = m_RenderQueue.create_uniform(nullptr, Uniform::MVP, m_Camera.get_mvp(model_matrix));
    
    auto* t1 = m_RenderQueue.create_texture(nullptr, m_Texture2.get(), Uniform::Texture0);

    packet.textures = t1;
    packet.uniforms = u;

    packet.vbo = m_VertexBuffer.get();
    packet.ibo = m_IndexBuffer.get();
    packet.shader = m_Shaders.get();
    packet.topology = GL_TRIANGLES;
    packet.primitive_start = 0;
    packet.primitive_end = 58806 / 3;

    RenderPacket packet2;
    Matrix4f model_matrix2;
    model_matrix2.InitTranslationTransform(Vector3f{ 0.0f, 0.0f, -50.0f });

    u = m_RenderQueue.create_uniform(nullptr, Uniform::MVP, m_Camera.get_mvp(model_matrix2));
    auto* t2 = m_RenderQueue.create_texture(nullptr, m_Texture.get(), Uniform::Texture0);

    packet2.uniforms = u;
    packet2.textures = t2;

    packet2.vbo = m_VertexBuffer.get();
    packet2.ibo = m_IndexBuffer.get();
    packet2.shader = m_Shaders.get();
    packet2.topology = GL_TRIANGLES;
    packet2.primitive_start = 0;
    packet2.primitive_end = 58806 / 3;

    m_RenderQueue.push_render_packet(packet);
    m_RenderQueue.push_render_packet(packet2);
 
    m_RenderQueue.draw_all();
    m_RenderQueue.clear();
}

void Engine::key_callback(ApplicationBase* app, ApplicationBaseKey key, ApplicationBaseKeyAction action)
{
    Engine* handler = reinterpret_cast<Engine*>(app);

    bool pressed = action == APP_KEY_PRESS;
    bool released = action == APP_KEY_RELEASE;

    bool active = pressed || !released;
    handler->on_key(key, active);
}

void Engine::framebuffer_callback(ApplicationBase* app, std::size_t width, std::size_t height)
{
    Engine* handler = reinterpret_cast<Engine*>(app);
    handler->m_Camera.change_framebuff_dimensions(width, height);
    handler->m_Camera.update_camera_matrices();
}

void Engine::on_key(ApplicationBaseKey key, bool pressed)
{
    switch (key)
    {
        case APP_KEY_W: m_Movement.up = pressed; break;
        case APP_KEY_S: m_Movement.down = pressed; break;
        case APP_KEY_A: m_Movement.left = pressed; break;
        case APP_KEY_D: m_Movement.right = pressed; break;
        case APP_KEY_Q: m_Movement.yaw_left = pressed; break;
        case APP_KEY_E: m_Movement.yaw_right = pressed; break;
        case APP_KEY_Z: m_Movement.pitch_up = pressed; break;
        case APP_KEY_X: m_Movement.pitch_down = pressed; break;

        default: break;
    };
}

void Engine::update_offset(float delta_seconds)
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
