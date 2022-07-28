#pragma once

#include "OpenGL.h"

enum ApplicationBaseKey
{
    APP_KEY_0,
    APP_KEY_1,
    APP_KEY_2,
    APP_KEY_3,
    APP_KEY_4,
    APP_KEY_5,
    APP_KEY_6,
    APP_KEY_7,
    APP_KEY_8,
    APP_KEY_9,

    APP_KEY_A,
    APP_KEY_B,
    APP_KEY_C,
    APP_KEY_D,
    APP_KEY_E,
    APP_KEY_F,
    APP_KEY_G,
    APP_KEY_H,
    APP_KEY_I,
    APP_KEY_J,
    APP_KEY_K,
    APP_KEY_L,
    APP_KEY_M,
    APP_KEY_N,
    APP_KEY_O,
    APP_KEY_P,
    APP_KEY_Q,
    APP_KEY_R,
    APP_KEY_S,
    APP_KEY_T,
    APP_KEY_U,
    APP_KEY_V,
    APP_KEY_W,
    APP_KEY_X,
    APP_KEY_Y,
    APP_KEY_Z,
 
    APP_KEY_TAB,
    APP_KEY_ESC,
    APP_KEY_UP,
    APP_KEY_DOWN,
    APP_KEY_LEFT,
    APP_KEY_RIGHT,
    APP_KEY_LSHIFT,
    APP_KEY_RSHIFT,
    APP_KEY_LCTRL,
    APP_KEY_RCTRL,

    APP_KEY_COUNT
};

enum ApplicationBaseKeyAction
{
    APP_KEY_NONE = 0,
    APP_KEY_PRESS,
    APP_KEY_RELEASE,
};

enum ApplicationBaseMouseButton
{
    APP_MOUSE_LEFT = 0,
    APP_MOUSE_RIGHT,
    APP_MOUSE_MIDDLE,

    APP_MOUSE_COUNT
};

inline static ApplicationBaseKey get_key(int keycode)
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
    };

    return APP_KEY_COUNT;
}

inline static ApplicationBaseKeyAction get_key_action(int action)
{
    switch (action)
    {
        case GLFW_PRESS:    return APP_KEY_PRESS;
        case GLFW_RELEASE:  return APP_KEY_RELEASE;
    }

    return APP_KEY_NONE;
}

inline static ApplicationBaseMouseButton get_button(int button)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:    return APP_MOUSE_LEFT;
        case GLFW_MOUSE_BUTTON_RIGHT:   return APP_MOUSE_RIGHT;
        case GLFW_MOUSE_BUTTON_MIDDLE:  return APP_MOUSE_MIDDLE;
    };

    return APP_MOUSE_COUNT;
}