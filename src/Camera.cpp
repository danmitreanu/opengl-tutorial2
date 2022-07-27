#include "Camera.h"

#include "Math3d.h"

Camera::Camera()
{
    m_ProjInfo.FOV = 75.0f;
    m_ProjInfo.zNear = 10.0f;
    m_ProjInfo.zFar = 10000.0f;
}

void Camera::change_framebuff_dimensions(std::size_t width, std::size_t height)
{
    m_ProjInfo.Width = width;
    m_ProjInfo.Height = height;
    m_Projection.InitPersProjTransform(m_ProjInfo);
}

void Camera::set( const Vector3f& look_at)
{
    m_LookAt = look_at;
}

const Matrix4f& Camera::get_view_matrix() const
{
    return m_View;
}

const Matrix4f& Camera::get_projection_matrix() const
{
    return m_Projection;
}

Matrix4f Camera::get_mvp(const Matrix4f& model) const
{
    return m_Projection * m_View * model;
}

Matrix4f Camera::get_vp() const
{
    return m_Projection * m_View;
}

void Camera::on_yaw(float rad)
{
    m_Yaw += rad;
}

void Camera::on_pitch(float rad)
{
    const float half_pi = 89.0f;
    m_Pitch += rad;

    // clamp between -pi/2 ; pi/2
    if (m_Pitch > half_pi)
        m_Pitch = half_pi;

    if (m_Pitch < -half_pi)
        m_Pitch = -half_pi;
}

void Camera::on_up_key(float up)
{
    m_LookAt.x += up;
}

void Camera::on_down_key(float up)
{
    on_up_key(-up);
}

void Camera::on_move_forward(float distance)
{
    Vector3f dir{ 1.0f, 0.0f, 0.0f };
 
    dir.Rotate(m_Yaw, m_Z);

    m_LookAt = m_LookAt + dir * distance;
}

void Camera::on_move_side(float distance)
{
    Vector3f dir{ 0.0f, 1.0f, 0.0f };

    dir.Rotate(m_Yaw, m_Z);

    m_LookAt = m_LookAt + dir * distance;
}

void Camera::update_camera_matrices()
{
    Vector3f dir{ 1.0f, 0.0f, 0.0f };
 
    dir.Rotate(m_Pitch, m_Y);
    dir.Rotate(m_Yaw, m_Z);

    m_View.InitCameraTransform(m_LookAt + dir * m_Distance, m_LookAt, m_Up);
}
