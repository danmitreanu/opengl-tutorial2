#pragma once

#include <iostream>

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
};

class ApplicationBase
{
private:
    GLFWwindow* m_Window = nullptr;
    // Width and height for the framebuffer

    WindowState m_WindowState;

    ResourceManager m_ResourceManager;

    static void key_callback(GLFWwindow*, int, int, int, int);
    static void framebuffer_size_callback(GLFWwindow*, int, int);
    static void focus_callback(GLFWwindow*, int);

    void on_window_resize(std::size_t, std::size_t);

    static ApplicationBaseKey get_key(int);
    static ApplicationBaseKeyAction get_key_action(int);

protected:
    bool init_window(const char*, std::size_t, std::size_t);
    inline std::size_t get_width() { return m_WindowState.width; }
    inline std::size_t get_height() { return m_WindowState.height; }

    void hide_mouse();
    void reset_mouse_pos();
    Vector2f get_mouse_offset();

    virtual void update(float) = 0;
    virtual void render() = 0;

    inline ResourceManager& get_resource_manager() { return m_ResourceManager; }

    virtual void key_callback(ApplicationBase*, ApplicationBaseKey, ApplicationBaseKeyAction) {}
    virtual void framebuffer_callback(ApplicationBase*, std::size_t, std::size_t) {}
    virtual void focus_callback(ApplicationBase*, bool) {}

public:
    ApplicationBase() = default;
    ~ApplicationBase();
    ApplicationBase& operator=(const ApplicationBase&) = delete;

    virtual void run();
};
