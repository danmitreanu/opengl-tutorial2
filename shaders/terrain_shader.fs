#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec2 SplatCoords;
in float FragHeight;
in vec3 FragNormal;

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D SplatMapTexture;

void main()
{
    vec4 grass = texture(Texture0, TexCoords).rgba;
    vec4 rock1 = texture(Texture1, TexCoords).rgba;
    vec4 rock2 = texture(Texture2, TexCoords).rgba;
    vec4 snow = texture(Texture3, TexCoords).rgba;

    vec4 splat = texture(SplatMapTexture, SplatCoords);

    float a = FragHeight;

    vec4 tex = mix(snow, rock1, splat.b);
    tex = mix(tex, rock2, splat.g);
    tex = mix(tex, grass, splat.r);

    FragColor = vec4(tex);

    //FragColor = vec4(normalize(FragNormal) * 0.5 + 0.5, 1.0);
}
