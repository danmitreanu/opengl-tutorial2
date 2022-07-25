#version 330 core

out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoords;
in vec2 SplatCoords;
in float FragHeight;

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4; // SplatMap

void main()
{
    vec4 grass = texture(Texture0, TexCoords).rgba;
    vec4 rock1 = texture(Texture1, TexCoords).rgba;
    vec4 rock2 = texture(Texture2, TexCoords).rgba;
    vec4 snow = texture(Texture3, TexCoords).rgba;

    vec4 splat = texture(Texture4, SplatCoords);

    //rock1 = mix(rock1, vec4(0.0, 0.5, 0.0, 1.0), 0.5);
    //grass = mix(grass, vec4(0.0, 1.0, 0.0, 1.0), 0.2);

    float a = FragHeight;
    /*vec4 black = vec4(0.0, 0.0, 0.0, 0.3);
    vec4 tex = mix(black, grass, log(a + 1) + 0.3);
    tex = mix(tex, rock1, pow(a, 8.0));
    tex = mix(tex, rock2, a);
    tex = mix(tex, snow, pow(a, 4.0));*/

    vec4 tex = mix(snow, rock1, splat.b);
    tex = mix(tex, rock2, splat.g);
    tex = mix(tex, grass, splat.r);

    FragColor = vec4(tex);
}
