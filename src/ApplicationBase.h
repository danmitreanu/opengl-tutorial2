#pragma once

#include <iostream>
#include <array>

#include "OpenGL.h"
#include "ResourceManager.h"

#include "ApplicationBaseKeys.h"

struct WindowState
{
    std::size_t width = 0;
    std::size_t height = 0;
    float framebuffer_scale = 1.0f;
    float midpoint[2] = { 0.0f, 0.0f };
    bool resized = false;

    float mouse_delta[2] = { 0.0f, 0.0f };
    float mouse_pos[2] = { 0.0f, 0.0f };
    float last_mouse_pos[2] = { 0.0f, 0.0f };
    float mouse_scroll = 0.0f;
    std::array<bool, (std::size_t)ApplicationBaseKey::APP_KEY_COUNT> pressed_keys;

    bool pitchyaw = false;
    bool focused = false;

    bool changed_mouse_scroll = false;
    bool changed_mouse_position = false;
    bool changed_window_size = false;
    bool changed_pitchyaw = false;
    bool changed_window_focused = false;

    inline bool is_pressed(const ApplicationBaseKey& key) const
    {
        return pressed_keys[key];
    }

    bool should_close = false;

    void reset();
};

class ApplicationBase
{
private:
    GLFWwindow* m_Window = nullptr;
    WindowState m_WindowState;

protected:
    bool init_window(const char*, std::size_t, std::size_t);
    inline std::size_t get_width() { return m_WindowState.width; }
    inline std::size_t get_height() { return m_WindowState.height; }

    virtual void update(const WindowState&, float) = 0;
    virtual void render() = 0;

public:
    virtual void on_window_resize(std::size_t, std::size_t);
    virtual void on_window_focus(const bool);

    virtual void on_key_down(const ApplicationBaseKey&);
    virtual void on_key_up(const ApplicationBaseKey&);

    virtual void on_mouse_pos_changed(const float, const float);
    virtual void on_mouse_button(const std::size_t, const bool);
    virtual void on_mouse_scroll(const float);

protected:
    virtual void on_mouse_delta(const float, const float);
    virtual void on_mouse_move(const float fx, const float fy, const float x, const float y);
    virtual void on_key_press_changed(const ApplicationBaseKey&, const bool) {}

public:
    ApplicationBase() = default;
    ~ApplicationBase();
    ApplicationBase& operator=(const ApplicationBase&) = delete;

    virtual void run();
    void close();
};
