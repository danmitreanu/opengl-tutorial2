#pragma once

#include <iostream>

#include "OpenGL.h"
#include "ResourceManager.h"

#include "ApplicationBaseKeys.h"

class ApplicationBase
{
private:
    GLFWwindow* m_Window = nullptr;
    // Width and height for the framebuffer
    std::size_t m_Width;
    std::size_t m_Height;

    ResourceManager m_ResourceManager;

    static void key_callback(GLFWwindow*, int, int, int, int);
    static void framebuffer_size_callback(GLFWwindow*, int, int);

    static ApplicationBaseKey get_key(int);
    static ApplicationBaseKeyAction get_key_action(int);

protected:
    bool init_window(const char*, std::size_t, std::size_t);
    inline std::size_t get_width() { return m_Width; }
    inline std::size_t get_height() { return m_Height; }

    virtual void update(float) = 0;
    virtual void render() = 0;

    inline ResourceManager& get_resource_manager() { return m_ResourceManager; }

    virtual void key_callback(ApplicationBase*, ApplicationBaseKey, ApplicationBaseKeyAction) {}
    virtual void framebuffer_callback(ApplicationBase*, std::size_t, std::size_t) {}

public:
    ApplicationBase() = default;
    ApplicationBase& operator=(const ApplicationBase&) = delete;

    virtual void run();
};