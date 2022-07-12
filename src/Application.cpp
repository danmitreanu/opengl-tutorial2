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

    m_Width = width;
    m_Height = height;

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
    glfwSetFramebufferSizeCallback(m_Window, Application::framebuffer_size_callback);
    glfwSetWindowUserPointer(m_Window, this);

    int fb_width, fb_height;
    glfwGetFramebufferSize(m_Window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);
    m_Width = fb_width;
    m_Height = fb_height;

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

void Application::init_camera()
{
    m_Camera.change_framebuff_dimensions(m_Width, m_Height);

    Vector3f look_at{ 0.0, 0.0f, 0.0f };
    
    m_Camera.set(look_at);
}

bool Application::initialize(const char* window_name, std::size_t width, std::size_t height)
{
    if (!init_glfw(window_name, width, height))
        return false;
 
    float data[] = {
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    float cube_data[] = {
        // face 1
        // triangle 1
        -3, -3, -3, 1, 0, 0,
        -3, 3, -3, 0, 1, 0,
        3, 3, -3, 0, 0, 1,
        // triangle 2
        -3, -3, -3, 1, 0, 0,
        3, -3, -3, 0, 1, 0,
        3, 3, -3, 0, 0, 1,

        // face 2
        // triangle 3
        -3, 3, 3, 1, 0, 0,
        -3, 3, -3, 0, 1, 0,
        3, 3, -3, 0, 0, 1,
        // triangle 4
        -3, 3, 3, 1, 0, 0,
        3, 3, 3, 0, 1, 0,
        3, 3, -3, 0, 0, 1,

        // face 3
        // triangle 5
        -3, -3, 3, 1, 0, 0,
        -3, 3, 3, 0, 1, 0,
        3, 3, 3, 0, 0, 1,
        // triangle 6
        3, -3, 3, 1, 0, 0,
        -3, -3, 3, 0, 1, 0,
        3, 3, 3, 0, 0, 1,

        // face 4
        // triangle 7
        3, 3, 3, 1, 0, 0,
        3, 3, -3, 0, 1, 0,
        3, -3, 3, 0, 0, 1,
        // triangle 8
        3, -3, 3, 1, 0, 0,
        -3, -3, 3, 0, 1, 0,
        3, -3, -3, 0, 0, 1,

        // face 5
        // triangle 9
        -3, -3, 3, 1, 0, 0,
        -3, -3, -3, 0, 1, 0,
        3, -3, -3, 0, 0, 1,
        // triangle 10
        3, -3, 3, 1, 0, 0,
        -3, -3, 3, 0, 1, 0,
        3, -3, -3, 0, 0, 1,

        // face 6
        // triangle 11
        -3, 3, 3, 1, 0, 0,
        -3, 3, -3, 0, 1, 0,
        -3, -3, -3, 0, 0, 1,
        //triangle 12
        -3, -3, 3, 1, 0, 0,
        -3, 3, 3, 0, 1, 0,
        -3, -3, -3, 0, 0, 1
    };

    init_buffer((void*)cube_data, sizeof(cube_data));
    init_shader();
    init_camera();

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
    update_offset(delta_seconds);

    m_ModelMatrix.InitIdentity();
    m_ModelMatrix.InitTranslationTransform(m_Offset.x, m_Offset.y, 0.0f);
    
    m_Camera.update_camera_matrices();
}

void Application::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
 
    m_VertexBuffer->bind();
    m_Shaders->bind();
 
    m_Shaders->set_uniform(Uniform::MVP, m_Camera.get_mvp(m_ModelMatrix));
 
    glDrawArrays(GL_TRIANGLES, 0, 36);

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

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Application* handler = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    handler->m_Width = width;
    handler->m_Height = height;
    handler->m_Camera.change_framebuff_dimensions(width, height);

    glViewport(0, 0, width, height);
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
            
            
        case GLFW_KEY_1: m_Movement.tab = true; break;
        case GLFW_KEY_2: m_Movement.ctrl = true; break;
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
            
        case GLFW_KEY_1: m_Movement.tab = false; break;
        case GLFW_KEY_2: m_Movement.ctrl = false; break;
    }
}

void Application::update_offset(float delta_seconds)
{
    const float speed = 1.0f; // units per sec
    const float camera_coeff = 125.0f;

    float diff = delta_seconds * speed;

    if(m_Movement.yaw_left != m_Movement.yaw_right)
    {
        float yaw_dir = -1.0f * int(m_Movement.yaw_left) + 1.0f * int(m_Movement.yaw_right);
        m_Camera.on_yaw(yaw_dir * diff * camera_coeff);
    }
    if(m_Movement.pitch_up != m_Movement.pitch_down)
    {
        float pitch_dir = -1.0f * int(m_Movement.pitch_up) + 1.0f * int(m_Movement.pitch_down);
        m_Camera.on_pitch(pitch_dir * diff * camera_coeff);
    }
    
    if(m_Movement.up != m_Movement.down)
    {
        float direction = 1.0f * int(m_Movement.up) - 1.0f * int(m_Movement.down);
        m_Camera.on_move_forward(direction * diff * camera_coeff);
    }
}
