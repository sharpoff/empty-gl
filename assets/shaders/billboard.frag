#version 460 core

in vec2 outUV;

out vec4 fragColor;

uniform vec3 color;
uniform sampler2D tex;
uniform bool useTexture;

void main()
{
    if (useTexture)
        fragColor = vec4(color, 1.0) * texture(tex, outUV);
    else
        fragColor = vec4(color, 1.0);
}
