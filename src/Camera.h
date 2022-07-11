#pragma once

#include "Math3D.h"

class Camera
{
private:
    Vector3f m_Position;
    Vector3f m_LookAt;

    Matrix4f m_View;
    Matrix4f m_Projection;

    Vector3f m_Up{ 0.0f, 0.0f, 1.0f }; // Z is up

    void update_view();
    void update_projection();

public:
    Camera() = default;

    void set_position(const Vector3f&);
    void set_look_at(const Vector3f&);

    Matrix4f get_view_matrix();
    Matrix4f get_projection_matrix();
};