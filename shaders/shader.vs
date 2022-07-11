#version 330 core

layout (location = 0) in vec2 Position;
layout (location = 1) in vec3 Color;

out vec3 VertexColor;

uniform vec2 Offset;

void main()
{
    gl_Position = vec4(Position.xy + Offset.xy, 0.0, 1.0);
    //VertexColor = Color;
    VertexColor = Color;
}
