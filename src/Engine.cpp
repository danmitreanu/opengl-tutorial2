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

bool Engine::initialize(const char* window_name, std::size_t width, std::size_t height)
{
    if (!init_window(window_name, width, height))
        return false;

    init_camera();
    init_texture();
    init_terrain();

    return true;
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
    m_HeightMap = m_ResourceManager.get_heightmap("heightmap.tga");

    m_Terrain = std::make_shared<Terrain>();
    m_Terrain->init_resources(&m_ResourceManager);
    m_Terrain->load_heightmap(m_HeightMap);
    m_Terrain->generate();
}

void Engine::update(
    const WindowState& window_state,
    InputState& input_state,
    const float delta_seconds)
{
    if (window_state.size_changed)
    {
        m_Camera.change_framebuff_dimensions(window_state.width, window_state.height);
    }

    if (input_state.key_pressed(APP_KEY_ESC))
    {
        if (m_Active)
        {
            m_Active = false;
            input_state.release_mouse();
        }
        else
        {
            close();
        }
    }

    if (input_state.mouse_clicked(APP_MOUSE_LEFT) && !m_Active)
    {
        m_Active = true;
        input_state.capture_mouse();
    }

    if (m_Active)
        update_movement(input_state, delta_seconds);

    m_Camera.update_camera_matrices();
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

void Engine::update_movement(InputState& input_state, const float delta_seconds)
{
    const float speed = input_state.key_pressed(APP_KEY_LSHIFT) ? 1500.0f : 800.0f;
    const float camera_coeff = 2.0f;

    const float mouse_offx = 0.0f;
    const float mouse_offy = 0.0f;

    const float move_diff = delta_seconds * speed;
    const float yaw_diff = delta_seconds * input_state.mouse_delta.x;
    const float pitch_diff = delta_seconds * input_state.mouse_delta.y;

    if (yaw_diff != 0)
    {
        m_Camera.on_yaw(yaw_diff * camera_coeff);
    }

    if (pitch_diff != 0)
    {
        m_Camera.on_pitch(pitch_diff * camera_coeff);
    }
 
    bool up = input_state.key_down(APP_KEY_W);
    bool down = input_state.key_down(APP_KEY_S);
    if (up != down)
    {
        float direction = -1.0f * int(up) + 1.0f * int(down);
        m_Camera.on_move_forward(direction * move_diff);
    }

    bool left = input_state.key_down(APP_KEY_A);
    bool right = input_state.key_down(APP_KEY_D);
    if (left != right)
    {
        float direction = 1.0f * int(left) - 1.0f * int(right);
        m_Camera.on_move_side(direction * move_diff);
    }
}

void Engine::before_run(const WindowState& window_state, InputState& input_state)
{
    m_Camera.change_framebuff_dimensions(window_state.width, window_state.height);
    input_state.capture_mouse();
}
