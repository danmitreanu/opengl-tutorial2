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
#include "Terrain.h"
#include "HeightMap.h"

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

void Engine::init_camera()
{
    m_Camera.change_framebuff_dimensions(get_width(), get_height());

    Vector3f look_at{ 0.0f, -50.0f, 0.0f };
 
    m_Camera.set(look_at);
}

void Engine::init_texture()
{
    m_Texture = m_ResourceManager.get_texture("mc.jpeg");
    m_Texture2 = m_ResourceManager.get_texture("grass.jpeg");
}

void Engine::init_terrain()
{
    m_HeightMap = std::make_shared<HeightMap>();
#ifndef _WIN32
    m_HeightMap->load("/Users/danm3/opengl/cmake/resources/heightmap.tga");
#else
    m_HeightMap->load("X:\\opengl-tutorial2\\resources\\height_map.jpeg");
#endif

    m_Terrain = std::make_shared<Terrain>();
    m_Terrain->init_resources(&m_ResourceManager);
    m_Terrain->load_heightmap(m_HeightMap);
    m_Terrain->generate();
}

bool Engine::initialize(const char* window_name, std::size_t width, std::size_t height)
{
    if (!init_window(window_name, width, height))
        return false;

    hide_mouse();

    init_buffer();
    init_camera();
    init_texture();
    init_terrain();

    return true;
}

void Engine::update(const float delta_seconds)
{
    auto mouse = get_mouse_offset();
    update_movement(delta_seconds, mouse);

    m_Camera.update_camera_matrices();
    reset_mouse_pos();
}

void Engine::render()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.4f, 0.6f, 0.7f, 0.0f);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix4f model;
    model.InitTranslationTransform(Vector3f{ -1.0f * m_Terrain->get_width() / 2.0f, -1.0f * m_Terrain->get_height() / 2.0f, -50.0f });
    auto* u = m_RenderQueue.create_uniform(nullptr, Uniform::MVP, m_Camera.get_mvp(model));

    auto packets = m_Terrain->get_packets(&m_RenderQueue, u);

    for (auto& packet : packets)
    {
        m_RenderQueue.push_render_packet(packet);
    }
 
    m_RenderQueue.draw_all();
    m_RenderQueue.clear();
}

void Engine::key_callback(ApplicationBase* app, ApplicationBaseKey key, ApplicationBaseKeyAction action)
{
    Engine* handler = reinterpret_cast<Engine*>(app);

    if (key == APP_KEY_ESC)
        close();

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

void Engine::focus_callback(ApplicationBase*, bool)
{
    hide_mouse();
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
        case APP_KEY_LSHIFT: m_Movement.shift = pressed; break;

        default: break;
    };
}

void Engine::update_movement(float delta_seconds, const Vector2f& mouse_offset)
{
    const float speed = m_Movement.shift ? 1500.0f : 800.0f;
    const float camera_coeff = 2.0f;

    float move_diff = delta_seconds * speed;
    float yaw_diff = delta_seconds * mouse_offset.x;
    float pitch_diff = delta_seconds * mouse_offset.y;

    float yratio = (float)get_width() / get_height();

    if (yaw_diff != 0)
    {
        m_Camera.on_yaw(yaw_diff * camera_coeff);
    }

    if (pitch_diff != 0)
    {
        m_Camera.on_pitch(yratio * pitch_diff * camera_coeff);
    }
 
    if (m_Movement.up != m_Movement.down)
    {
        float direction = -1.0f * int(m_Movement.up) + 1.0f * int(m_Movement.down);
        m_Camera.on_move_forward(direction * move_diff);
    }

    if (m_Movement.left != m_Movement.right)
    {
        float direction = 1.0f * int(m_Movement.left) - 1.0f * int(m_Movement.right);
        m_Camera.on_move_side(direction * move_diff);
    }
}
