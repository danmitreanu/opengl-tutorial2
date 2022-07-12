#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

out vec3 VertexColor;

uniform vec2 Offset;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 MVP;

void main()
{
    //gl_Position = Model * View * Projection * vec4(Position.xy + Offset.xy, 0.0, 1.0);
    // gl_Position = Projection * View * Model * vec4(Position.xy + Offset.xy, Position.z, 1.0);

    gl_Position = MVP * vec4(Position, 1.0);
    VertexColor = Color;
}
