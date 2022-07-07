#version 330 core

out vec4 FragColor;

in vec3 VertexColor;

void main()
{
    FragColor = vec4(VertexColor.x, VertexColor.y, VertexColor.z, 0.0);
}
