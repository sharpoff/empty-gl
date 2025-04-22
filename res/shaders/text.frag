#version 330 core

uniform sampler2D text;
uniform vec3 textColor;

in vec2 texUV;
out vec4 fragColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texUV).r);
    fragColor = vec4(textColor, 1.0) * sampled;
}
