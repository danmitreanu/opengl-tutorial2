#include "Camera.h"

#include "Math3d.h"

void Camera::set_position(const Vector3f& xyz)
{
    m_Position = xyz;
}

void Camera::set_look_at(const Vector3f& xyz)
{
    m_LookAt = xyz;
}

void Camera::update_view()
{
    m_View.InitCameraTransform(m_Position, m_LookAt, m_Up);
}

void Camera::update_projection()
{

}