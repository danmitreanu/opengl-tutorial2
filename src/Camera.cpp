#include "Camera.h"

#include "Math3d.h"

Camera::Camera()
{
    m_ProjInfo.FOV = 90;
    m_ProjInfo.zNear = 0.3f;
    m_ProjInfo.zFar = 1000.0f;
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

const Matrix4f& Camera::get_view_matrix()
{
    return m_View;
}

const Matrix4f& Camera::get_projection_matrix()
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
    m_Pitch += rad;
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
    Vector3f dir = Vector3f(1.0f,0.0f, 0.0f);
    
    dir.Rotate(m_Yaw, Vector3f(0.0f,0.0f, 1.0f));
    
    m_LookAt = m_LookAt + dir * distance;
}

void Camera::update_camera_matrices()
{
    Vector3f dir = Vector3f(1.0f,0.0f, 0.0f);
   
    dir.Rotate(m_Pitch, Vector3f(0.0f,1.0f, 0.0f));
    dir.Rotate(m_Yaw, Vector3f(0.0f,0.0f, 1.0f));
    //m_Position.x = m_LookAt.x + m_Distance * sin(m_Rotation);
    //m_Position.y = m_LookAt.y + m_Distance * cos(m_Rotation);

    m_View.InitCameraTransform(m_LookAt + dir * m_Distance, m_LookAt, m_Up);
}
