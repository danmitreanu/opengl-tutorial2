#pragma once

#include <iostream>
#include <array>

#include "OpenGL.h"
#include "ResourceManager.h"
#include "Math3D.h"

#include "ApplicationBaseInputs.h"

struct WindowState
{
    std::size_t width = 0;
    std::size_t height = 0;
    float framebuffer_scale = 1.0f;
    float midx = 0.0f;
    float midy = 0.0f;

    bool focused = true;

    bool should_close = false;

    bool size_changed = false;
    bool focus_changed = false;

    void reset();
};

struct InputState
{
private:
    GLFWwindow* m_Window = nullptr;

    std::array<bool, (std::size_t)APP_KEY_COUNT> m_KeysPressed = { false };
    std::array<bool, (std::size_t)APP_KEY_COUNT> m_KeysDown = { false };

    std::array<bool, (std::size_t)APP_MOUSE_COUNT> m_ButtonsClicked = { false };
    std::array<bool, (std::size_t)APP_MOUSE_COUNT> m_ButtonsDown = { false };

    friend class ApplicationBase;
    friend void glfw_key_press_callback(GLFWwindow*, int, int, int, int);
    friend void glfw_cursor_pos_callback(GLFWwindow*, double, double);
    friend void glfw_framebuffer_size_callback(GLFWwindow*, int, int);
    friend void glfw_mouse_button_callback(GLFWwindow*, int, int, int);
    friend void glfw_window_focus_callback(GLFWwindow*, int);

public:
    void capture_mouse();
    void release_mouse();

    bool mouse_captured = false;
    Vector2f mouse_pos{ 0.0f, 0.0f };
    Vector2f mouse_delta{ 0.0f, 0.0f };

    bool key_pressed(ApplicationBaseKey);
    inline bool key_down(ApplicationBaseKey key) { return m_KeysDown[key]; }

    // ensures click event is true only once per click when checked in update()
    bool mouse_clicked(ApplicationBaseMouseButton);
    inline bool mouse_down(ApplicationBaseMouseButton button) { return m_ButtonsDown[button]; }

    void reset();
};

class ApplicationBase
{
private:
    GLFWwindow* m_Window = nullptr;
    WindowState m_WindowState;
    InputState m_InputState;

    friend void glfw_key_press_callback(GLFWwindow*, int, int, int, int);
    friend void glfw_cursor_pos_callback(GLFWwindow*, double, double);
    friend void glfw_framebuffer_size_callback(GLFWwindow*, int, int);
    friend void glfw_mouse_button_callback(GLFWwindow*, int, int, int);
    friend void glfw_window_focus_callback(GLFWwindow*, int);

protected:
    virtual bool initialize(const char* window_name, std::size_t, std::size_t) = 0;
    bool init_window(const char*, std::size_t, std::size_t);
    inline std::size_t get_width() { return m_WindowState.width; }
    inline std::size_t get_height() { return m_WindowState.height; }

    virtual void update(const WindowState&, InputState&, float) = 0;
    virtual void render() = 0;

public:
    ApplicationBase() = default;
    ~ApplicationBase();
    ApplicationBase& operator=(const ApplicationBase&) = delete;

    virtual void before_run(const WindowState&, InputState&) {}
    void run();
    void close();
};

ApplicationBaseKey get_key(int);
ApplicationBaseKeyAction get_key_action(int);