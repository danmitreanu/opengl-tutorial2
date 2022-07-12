#include "Camera.h"

#include "Math3d.h"

Camera::Camera()
{
    m_ProjInfo.FOV = 90;
    m_ProjInfo.zNear = 1.0f;
    m_ProjInfo.zFar = 1000.0f;
}

void Camera::change_framebuff_dimensions(std::size_t width, std::size_t height)
{
    m_ProjInfo.Width = width;
    m_ProjInfo.Height = height;
}

void Camera::set(const Vector3f& pos, const Vector3f& look_at)
{
    m_Position = pos;
    m_LookAt = look_at;

    m_View.InitCameraTransform(m_Position, m_LookAt, m_Up);
    m_Projection.InitPersProjTransform(m_ProjInfo);
}

const Matrix4f& Camera::get_view_matrix()
{
    return m_View;
}

const Matrix4f& Camera::get_projection_matrix()
{
    return m_Projection;
}

Matrix4f Camera::get_mvp(const Matrix4f& model)
{
    return m_Projection * m_View * model;
}

Matrix4f Camera::get_mvp()
{
    Matrix4f model;
    model.InitIdentity();
    return get_mvp(model);
}