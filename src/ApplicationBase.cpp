#include "ApplicationBase.h"

bool ApplicationBase::init_glfw(const char* window_name, std::size_t width, std::size_t height)
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
    glfwSetWindowUserPointer(m_Window, this);

    int fb_width, fb_height;
    glfwGetFramebufferSize(m_Window, &fb_width, &fb_height);
    glfwSetKeyCallback(m_Window, ApplicationBase::key_callback);
    glfwSetFramebufferSizeCallback(m_Window, ApplicationBase::framebuffer_size_callback);
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

void ApplicationBase::set_key_callback(KeyCallback callback)
{
    m_KeyCallback = callback;
}

void ApplicationBase::set_framebuffer_callback(FramebuffCallback callback)
{
    m_FramebuffCallback = callback;
}

void ApplicationBase::key_callback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
    ApplicationBase* handler = reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));
    if (keycode == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    if (handler->m_KeyCallback)
        handler->m_KeyCallback(handler, keycode, action);
}

void ApplicationBase::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ApplicationBase* handler = reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));

    handler->m_Width = width;
    handler->m_Height = height;
    glViewport(0, 0, width, height);

    if (handler->m_FramebuffCallback)
        handler->m_FramebuffCallback(handler, width, height);
}

void ApplicationBase::run()
{
    float last_time = glfwGetTime();

    while (!glfwWindowShouldClose(m_Window))
    {
        float now_time = glfwGetTime();

        float delta_time = now_time - last_time;
        this->update(delta_time);
        this->render();
        last_time = now_time;

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}
