#version 330 core

out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoords;

uniform sampler2D Texture0;

void main()
{
    //FragColor = vec4(VertexColor.x, VertexColor.y, VertexColor.z, 0.0);
    FragColor = vec4(texture(Texture0, TexCoords).rgb,1.0);
}
