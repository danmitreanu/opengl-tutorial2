#version 330 core

in vec3 Position;
in vec3 Color;
in vec2 UV;

out vec3 VertexColor;
out vec2 TexCoords;
out vec2 SplatCoords;
out float FragHeight;

uniform mat4 MVP;

void main()
{
    float height = Position.z;

    gl_Position = MVP * vec4(Position.xy, pow(height, 1.3) * 800.0 - 400.0, 1.0);
    VertexColor = Color;
    SplatCoords = UV;
    TexCoords = UV * 20.0;
    FragHeight = Position.z;
}
