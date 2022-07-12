#pragma once

#include "Math3D.h"

struct Camera
{
private:
    //Vector3f m_Position;
    Vector3f m_LookAt;
    
    float m_Pitch = 0.0f;
    float m_Distance = 20.0f;
    float m_Yaw  = 0.0f;
    

    Matrix4f m_View;
    Matrix4f m_Projection;
    PersProjInfo m_ProjInfo;

    Vector3f m_Up{ 0.0f, 0.0f, 1.0f }; // Z is up

    
public:
    Camera();

    
    void update_camera_matrices();

    void change_framebuff_dimensions(std::size_t, std::size_t);

    void set( const Vector3f& look_at);

    const Matrix4f& get_view_matrix();
    const Matrix4f& get_projection_matrix();
    Matrix4f get_mvp(const Matrix4f& model) const;
    Matrix4f get_vp() const;

    void on_yaw(float);
    void on_pitch(float rad);
    void on_move_forward(float distance);
    
    
    //void move(float);
    void on_up_key(float);
    void on_down_key(float);
};
