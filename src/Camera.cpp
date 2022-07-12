#include "Camera.h"

#include "Math3d.h"

Camera::Camera()
{
    m_ProjInfo.FOV = 90;
    m_ProjInfo.zNear = 1.0f;
    m_ProjInfo.zFar = 10.0f;
}

void Camera::change_framebuff_dimensions(std::size_t width, std::size_t height)
{
    m_ProjInfo.Width = width;
    m_ProjInfo.Height = height;
}

void Camera::set_position(const Vector3f& xyz)
{
    m_Position = xyz;
    update_view();
}

void Camera::set_look_at(const Vector3f& xyz)
{
    m_LookAt = xyz;
    update_view();
}

void Camera::update_view()
{
    m_View.InitCameraTransform(m_Position, m_LookAt, m_Up);
}

void Camera::update_projection()
{
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