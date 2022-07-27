#include "ApplicationBase.h"
#include "ApplicationBaseKeys.h"

ApplicationBase::~ApplicationBase()
{
    glfwTerminate();
#ifdef _WIN32
    glewTerminate();
#endif
}

bool ApplicationBase::init_window(const char* window_name, std::size_t width, std::size_t height)
{
    if (!glfwInit())
        return false;

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
    glfwSetWindowFocusCallback(m_Window, ApplicationBase::focus_callback);
    glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glViewport(0, 0, fb_width, fb_height);


    m_WindowState.framebuffer_scale = (float)fb_width / width;
    on_window_resize((std::size_t)fb_width, (std::size_t)fb_height);

#ifdef _WIN32
    if (glewInit() != GLEW_OK)
    {
        std::cout << "WIN32 GLEW could not be initialized." << std::endl;
        return false;
    }
#endif

    return true;
}

void ApplicationBase::hide_mouse()
{
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ApplicationBase::reset_mouse_pos()
{
    glfwSetCursorPos(m_Window, m_WindowState.midpoint[0], m_WindowState.midpoint[1]);
}

Vector2f ApplicationBase::get_mouse_offset()
{
    double xpos, ypos;
    glfwGetCursorPos(m_Window, &xpos, &ypos);

    float offsetX = m_WindowState.midpoint[0] - xpos;
    float offsetY = m_WindowState.midpoint[1] - ypos;

    return Vector2f{ -1.0f * offsetX, offsetY };
}

void ApplicationBase::key_callback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
    ApplicationBase* handler = reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));
    if (keycode == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    auto app_key = get_key(keycode);
    auto app_action = get_key_action(action);
    handler->key_callback(handler, app_key, app_action);
}

void ApplicationBase::on_window_resize(std::size_t width, std::size_t height)
{
    if (m_WindowState.width != width || m_WindowState.height != height)
    {
        m_WindowState.width = width;
        m_WindowState.height = height;

        m_WindowState.midpoint[0] = width / (2.0f * m_WindowState.framebuffer_scale);
        m_WindowState.midpoint[1] = height / (2.0f * m_WindowState.framebuffer_scale);

        m_WindowState.resized = true;
    }
}

void ApplicationBase::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ApplicationBase* handler = reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));

    handler->on_window_resize((std::size_t)width, (std::size_t)height);
    glViewport(0, 0, width, height);

    handler->framebuffer_callback(handler, width, height);
}

void ApplicationBase::focus_callback(GLFWwindow* window, int focused)
{
    ApplicationBase* handler = reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));

    handler->focus_callback(handler, focused == GLFW_TRUE ? true : false);
}

ApplicationBaseKey ApplicationBase::get_key(int keycode)
{
    int ka = (int)APP_KEY_A;
    int k0 = (int)APP_KEY_0;

    if (keycode >= GLFW_KEY_A && keycode <= GLFW_KEY_Z)
    {
        return (ApplicationBaseKey)(ka + keycode - (int)GLFW_KEY_A);
    }

    if (keycode >= GLFW_KEY_0 && keycode <= GLFW_KEY_9)
    {
        return (ApplicationBaseKey)(k0 + keycode - (int)GLFW_KEY_0);
    }

    switch (keycode)
    {
        case GLFW_KEY_TAB:              return APP_KEY_TAB;
        case GLFW_KEY_ESCAPE:           return APP_KEY_ESC;
        case GLFW_KEY_UP:               return APP_KEY_UP;
        case GLFW_KEY_DOWN:             return APP_KEY_DOWN;
        case GLFW_KEY_LEFT:             return APP_KEY_LEFT;
        case GLFW_KEY_RIGHT:            return APP_KEY_RIGHT;
        case GLFW_KEY_LEFT_SHIFT:       return APP_KEY_LSHIFT;
        case GLFW_KEY_RIGHT_SHIFT:      return APP_KEY_RSHIFT;
        case GLFW_KEY_LEFT_CONTROL:     return APP_KEY_LCTRL;
        case GLFW_KEY_RIGHT_CONTROL:    return APP_KEY_RCTRL;

        default:                        return APP_KEY_UNKNOWN;
    };
}

ApplicationBaseKeyAction ApplicationBase::get_key_action(int action)
{
    switch (action)
    {
        case GLFW_PRESS:    return APP_KEY_PRESS;
        case GLFW_RELEASE:  return APP_KEY_RELEASE;

        default:            return APP_KEY_NONE;
    }
}

void ApplicationBase::run()
{
    reset_mouse_pos();
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

        m_WindowState.reset();
    }
}

void WindowState::reset()
{
    resized = false;
}