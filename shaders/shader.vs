#version 330 core

in vec3 Position;
in vec3 Color;
in vec2 UV;

out vec3 VertexColor;
out vec2 TexCoords;

uniform mat4 MVP;

void main()
{
    //gl_Position = Model * View * Projection * vec4(Position.xy + Offset.xy, 0.0, 1.0);
    // gl_Position = Projection * View * Model * vec4(Position.xy + Offset.xy, Position.z, 1.0);

    gl_Position = MVP * vec4(Position, 1.0);
    VertexColor = Color;
    TexCoords = UV;
}
