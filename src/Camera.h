#pragma once

#include "Math3D.h"

class Camera
{
private:
    Vector3f m_Position;
    Vector3f m_LookAt;

    Matrix4f m_View;
    Matrix4f m_Projection;
    PersProjInfo m_ProjInfo;

    Vector3f m_Up{ 0.0f, 0.0f, 1.0f }; // Z is up

    void update_view();
    void update_projection();

public:
    Camera();

    void change_framebuff_dimensions(std::size_t, std::size_t);

    void set_position(const Vector3f&);
    void set_look_at(const Vector3f&);

    const Matrix4f& get_view_matrix();
    const Matrix4f& get_projection_matrix();
};