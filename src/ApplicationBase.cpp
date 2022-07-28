#include "ApplicationBase.h"
#include "ApplicationBaseInputs.h"

inline ApplicationBase* glfw_get_base(GLFWwindow* window)
{
    return reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));
}

/* -------------- GLFW --------------- */

void glfw_key_press_callback(GLFWwindow*, int, int, int, int);
void glfw_cursor_pos_callback(GLFWwindow*, double, double);
void glfw_framebuffer_size_callback(GLFWwindow*, int, int);
void glfw_mouse_button_callback(GLFWwindow*, int, int, int);
void glfw_window_focus_callback(GLFWwindow*, int);

void glfw_key_press_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* handler = glfw_get_base(window);

    auto app_key = get_key(key);
    if (action == GLFW_PRESS)
    {
        handler->m_InputState.m_KeysPressed[app_key] = true;
        handler->m_InputState.m_KeysDown[app_key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        handler->m_InputState.m_KeysPressed[app_key] = false;
        handler->m_InputState.m_KeysDown[app_key] = false;
    }
}

void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    auto* handler = glfw_get_base(window);

    float x = (float)xpos, y = (float)ypos;

    // ensure identical mouse movement on all axes
    float yratio = (float)handler->m_WindowState.width / handler->m_WindowState.height;

    handler->m_InputState.mouse_delta.x = -1.0f * handler->m_InputState.mouse_pos.x + x;
    handler->m_InputState.mouse_delta.y = handler->m_InputState.mouse_pos.y - y;
    handler->m_InputState.mouse_delta.y *= yratio;

    handler->m_InputState.mouse_pos.x = x;
    handler->m_InputState.mouse_pos.y = y;
}

void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    auto* handler = glfw_get_base(window);
    glViewport(0, 0, width, height);

    handler->m_WindowState.width = (std::size_t)width;
    handler->m_WindowState.height = (std::size_t)height;
    handler->m_WindowState.midx = width / (2.0f * handler->m_WindowState.framebuffer_scale);
    handler->m_WindowState.midy = height / (2.0f * handler->m_WindowState.framebuffer_scale);

    handler->m_WindowState.size_changed = true;
}

void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    auto* handler = glfw_get_base(window);

    auto app_button = get_button(button);
    if (action == GLFW_PRESS)
    {
        handler->m_InputState.m_ButtonsClicked[app_button] = true;
        handler->m_InputState.m_ButtonsDown[app_button] = true;
    }
    else
    {
        handler->m_InputState.m_ButtonsClicked[app_button] = false;
        handler->m_InputState.m_ButtonsDown[app_button] = false;
    }
}

void glfw_window_focus_callback(GLFWwindow* window, int focused)
{
    auto* handler = glfw_get_base(window);

    bool focus = focused == GLFW_TRUE ? true : false;

    if (handler->m_WindowState.focused == focus)
        return;

    handler->m_WindowState.focused = focus;
    handler->m_WindowState.focus_changed = true;
}

/* -------------- /GLFW --------------- */

ApplicationBase::~ApplicationBase()
{
    glfwTerminate();
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

    m_InputState.m_Window = m_Window;

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, this);

    int fb_width, fb_height;
    glfwGetFramebufferSize(m_Window, &fb_width, &fb_height);
    m_WindowState.framebuffer_scale = (float)fb_width / width;
    glfw_framebuffer_size_callback(m_Window, fb_width, fb_height);

    glfwSetKeyCallback(m_Window, glfw_key_press_callback);
    glfwSetCursorPosCallback(m_Window, glfw_cursor_pos_callback);
    glfwSetFramebufferSizeCallback(m_Window, glfw_framebuffer_size_callback);
    glfwSetMouseButtonCallback(m_Window, glfw_mouse_button_callback);
    glfwSetWindowFocusCallback(m_Window, glfw_window_focus_callback);

#ifdef _WIN32
    if (glewInit() != GLEW_OK)
    {
        std::cout << "WIN32 GLEW could not be initialized." << std::endl;
        return false;
    }
#endif

    return true;
}

void ApplicationBase::run()
{
    before_run(m_WindowState, m_InputState);

    float last_time = glfwGetTime();

    while (!m_WindowState.should_close)
    {
        float now_time = glfwGetTime();

        float delta_time = now_time - last_time;
        this->update(m_WindowState, m_InputState, delta_time);
        this->render();
        last_time = now_time;

        m_InputState.reset();
        m_WindowState.reset();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void ApplicationBase::close()
{
    m_WindowState.should_close = true;
}

void WindowState::reset()
{
    size_changed = false;
    focus_changed = false;
}

bool InputState::key_pressed(ApplicationBaseKey key)
{
    if (!m_KeysPressed[key])
        return false;

    m_KeysPressed[key] = false;
    return true;
}

bool InputState::mouse_clicked(ApplicationBaseMouseButton button)
{
    if (!m_ButtonsClicked[button])
        return false;

    m_ButtonsClicked[button] = false;
    return true;
}

void InputState::capture_mouse()
{
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputState::release_mouse()
{
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void InputState::reset()
{
    mouse_delta.x = 0;
    mouse_delta.y = 0;
}
