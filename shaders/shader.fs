#version 330 core

out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoords;
in float FragHeight;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

void main()
{
    //FragColor = vec4(VertexColor.x, VertexColor.y, VertexColor.z, 0.0);
    /*if (FragHeight > -550)
        FragColor = vec4(texture(Texture0, TexCoords).rgb, 1.0);
    else
        FragColor = vec4(texture(Texture1, TexCoords).rgb, 1.0);*/

    if (FragHeight < -700)
        FragColor = vec4(texture(Texture1, TexCoords).rgb, 1.0);
    else
        FragColor = vec4(texture(Texture0, TexCoords).rgb, 1.0);
}
