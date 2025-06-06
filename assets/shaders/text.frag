#version 460 core

in vec2 outUV;

out vec4 fragColor;

uniform sampler2D tex;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, outUV).r);

    fragColor = vec4(textColor, 1.0) * sampled;
}
