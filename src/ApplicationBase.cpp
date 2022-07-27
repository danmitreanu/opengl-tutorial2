#include "ApplicationBase.h"
#include "ApplicationBaseKeys.h"

ApplicationBaseKey get_key(int keycode)
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

        default:                        return APP_KEY_COUNT;
    };
}

ApplicationBaseKeyAction get_key_action(int action)
{
    switch (action)
    {
        case GLFW_PRESS:    return APP_KEY_PRESS;
        case GLFW_RELEASE:  return APP_KEY_RELEASE;

        default:            return APP_KEY_NONE;
    }
}

void glfw_resize_callback(GLFWwindow* window, int width, int height)
{
    ApplicationBase* handler = reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));

    handler->on_window_resize((std::size_t)width, (std::size_t)height);
    glViewport(0, 0, width, height);

    //handler->framebuffer_callback(handler, width, height);
}

void glfw_focus_callback(GLFWwindow* window, int focused)
{
    ApplicationBase* handler = reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));

    handler->on_window_focus(focused == GLFW_TRUE ? true : false);
}

void glfw_key_callback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
    ApplicationBase* handler = reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));

    bool pressed = action == GLFW_PRESS;

    if (pressed)
        handler->on_key_down(get_key(keycode));
    else
        handler->on_key_up(get_key(keycode));
}

void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ApplicationBase* handler = reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));
    handler->on_mouse_scroll((float)yoffset);
}

void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ApplicationBase* handler = reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));

    static bool pressed_left = false;
    static bool pressed_right = false;

    const bool pressed = action == GLFW_PRESS;

    if (button == GLFW_MOUSE_BUTTON_LEFT && pressed != pressed_left)
    {
        pressed_left = pressed;
        handler->on_mouse_button(0, pressed);
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && pressed != pressed_right)
    {
        pressed_right = pressed;
        handler->on_mouse_button(1, pressed);
    }
}

void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    ApplicationBase* handler = reinterpret_cast<ApplicationBase*>(glfwGetWindowUserPointer(window));
    handler->on_mouse_pos_changed((float)xpos, (float)ypos);
}

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

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, this);

    int fb_width, fb_height;
    glfwGetFramebufferSize(m_Window, &fb_width, &fb_height);
    m_WindowState.framebuffer_scale = (float)fb_width / width;
    on_window_resize((std::size_t)fb_width, (std::size_t)fb_height);

    glfwSetFramebufferSizeCallback(m_Window, glfw_resize_callback);
    glfwSetKeyCallback(m_Window, glfw_key_callback);
    glfwSetScrollCallback(m_Window, glfw_scroll_callback);
    glfwSetMouseButtonCallback(m_Window, glfw_mouse_button_callback);
    glfwSetCursorPosCallback(m_Window, glfw_cursor_pos_callback);
    glfwSetWindowFocusCallback(m_Window, glfw_focus_callback);
    glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glViewport(0, 0, fb_width, fb_height);

#ifdef _WIN32
    if (glewInit() != GLEW_OK)
    {
        std::cout << "WIN32 GLEW could not be initialized." << std::endl;
        return false;
    }
#endif

    return true;
}

void ApplicationBase::on_window_resize(std::size_t width, std::size_t height)
{
    if (m_WindowState.width != width || m_WindowState.height != height)
    {
        m_WindowState.width = width;
        m_WindowState.height = height;

        m_WindowState.midpoint[0] = width / (2.0f * m_WindowState.framebuffer_scale);
        m_WindowState.midpoint[1] = height / (2.0f * m_WindowState.framebuffer_scale);

        m_WindowState.changed_window_size = true;
    }
}

void ApplicationBase::on_window_focus(const bool focused)
{
    if (m_WindowState.focused != focused)
    {
        m_WindowState.focused = focused;
        m_WindowState.changed_window_focused = true;
    }
}

void ApplicationBase::on_key_down(const ApplicationBaseKey& key)
{
    if (m_WindowState.pressed_keys[key] == false)
    {
        m_WindowState.pressed_keys[key] = true;
        on_key_press_changed(key, true);
    }
}

void ApplicationBase::on_key_up(const ApplicationBaseKey& key)
{
    if (key == APP_KEY_ESC)
    {
        if (m_WindowState.pitchyaw)
        {
            m_WindowState.pitchyaw = false;
            m_WindowState.changed_pitchyaw = true;
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
        }
    }

    if (m_WindowState.pressed_keys[key] == true)
    {
        m_WindowState.pressed_keys[key] = false;
        on_key_press_changed(key, false);
    }
}

void ApplicationBase::on_mouse_pos_changed(const float x, const float y)
{
    if (m_WindowState.pitchyaw)
    {
        float dx = x - m_WindowState.last_mouse_pos[0];
        float dy = y - m_WindowState.last_mouse_pos[1];
        on_mouse_delta(dx, dy);
    }
    else
    {
        on_mouse_move(m_WindowState.last_mouse_pos[0], m_WindowState.last_mouse_pos[1], x, y);
    }

    m_WindowState.changed_mouse_position = true;

    m_WindowState.last_mouse_pos[0] = x;
    m_WindowState.last_mouse_pos[1] = y;
}

void ApplicationBase::on_mouse_delta(const float x, const float y)
{
    m_WindowState.mouse_delta[0] = x;
    m_WindowState.mouse_delta[1] = y;
}

void ApplicationBase::on_mouse_move(const float fx, const float fy, const float x, const float y)
{
    m_WindowState.mouse_delta[0] = x - fx;
    m_WindowState.mouse_delta[1] = y - fy;

    m_WindowState.mouse_pos[0] = x;
    m_WindowState.mouse_pos[1] = y;
}

void ApplicationBase::on_mouse_button(const std::size_t button, const bool pressed)
{
    if (button == 0 && pressed && m_WindowState.pitchyaw == false)
    {
        m_WindowState.pitchyaw = true;
        m_WindowState.changed_pitchyaw = true;
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void ApplicationBase::on_mouse_scroll(const float delta)
{
    m_WindowState.mouse_scroll = delta;
    m_WindowState.changed_mouse_scroll = true;
}

void ApplicationBase::run()
{
    float last_time = glfwGetTime();

    while (!glfwWindowShouldClose(m_Window))
    {
        float now_time = glfwGetTime();

        float delta_time = now_time - last_time;
        this->update(m_WindowState, delta_time);
        this->render();
        last_time = now_time;

        glfwSwapBuffers(m_Window);
        glfwPollEvents();

        m_WindowState.reset();
    }
}

void ApplicationBase::close()
{
    m_WindowState.should_close = true;
}

void WindowState::reset()
{
    resized = false;
    mouse_delta[0] = 0.0f;
    mouse_delta[1] = 0.0f;

    changed_mouse_position = false;
    changed_mouse_scroll = false;
    changed_window_size = false;
    changed_window_focused = false;
    changed_pitchyaw = false;
}
