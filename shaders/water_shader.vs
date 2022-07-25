#version 330 core

in vec2 Position;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(Position.xy, 0.1 * 800.0 - 400.0, 1.0);
}
