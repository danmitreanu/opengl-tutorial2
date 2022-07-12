#pragma once

#include "Math3D.h"

struct Camera
{

    Vector3f m_Position;
    Vector3f m_LookAt;

    Matrix4f m_View;
    Matrix4f m_Projection;
    PersProjInfo m_ProjInfo;

    Vector3f m_Up{ 0.0f, 0.0f, 1.0f }; // Z is up

public:
    Camera();

    void change_framebuff_dimensions(std::size_t, std::size_t);

    void set(const Vector3f& pos, const Vector3f& look_at);

    const Matrix4f& get_view_matrix();
    const Matrix4f& get_projection_matrix();
    Matrix4f get_mvp(const Matrix4f& model);
    Matrix4f get_mvp();
};
