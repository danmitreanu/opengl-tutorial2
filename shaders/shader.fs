#version 330 core

out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoords;
in float FragHeight;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

void main()
{
    vec3 dirt = texture(Texture0, TexCoords).rgb;
    vec3 grass = texture(Texture1, TexCoords).rgb;

    float a = 2.5 * FragHeight;
    if (a > 1.0)
        a = 1.0;

    FragColor = vec4(mix(grass, dirt, a), 1.0);
}
