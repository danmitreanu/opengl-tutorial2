#pragma once

#include <iostream>

#include "OpenGL.h"
#include "ResourceManager.h"

typedef void (*KeyCallback)(void*, int, int);
typedef void (*FramebuffCallback)(void*, std::size_t, std::size_t);

class ApplicationBase
{
private:
    GLFWwindow* m_Window = nullptr;
    std::size_t m_Width;
    std::size_t m_Height;

    KeyCallback m_KeyCallback = nullptr;
    FramebuffCallback m_FramebuffCallback = nullptr;

    ResourceManager m_ResourceManager;

    static void key_callback(GLFWwindow*, int, int, int, int);
    static void framebuffer_size_callback(GLFWwindow*, int, int);

protected:
    bool init_glfw(const char*, std::size_t, std::size_t);
    inline std::size_t get_width() { return m_Width; }
    inline std::size_t get_height() { return m_Height; }

    virtual void update(float) = 0;
    virtual void render() = 0;

    inline ResourceManager& get_resource_manager() { return m_ResourceManager; }

    void set_key_callback(KeyCallback);
    void set_framebuffer_callback(FramebuffCallback);

public:
    ApplicationBase() = default;
    ApplicationBase& operator=(const ApplicationBase&) = delete;

    void run();
};