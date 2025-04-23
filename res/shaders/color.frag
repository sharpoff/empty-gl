#version 460 core

#extension GL_ARB_bindless_texture : require

out vec4 fragColor;

uniform vec3 color;

void main()
{
    fragColor = vec4(color, 1.0);
}
