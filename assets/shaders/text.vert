#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;

out vec2 outUV;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(inPos.xy, 0.0, 1.0);
    outUV = inUV;
}
