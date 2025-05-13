#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;

out vec2 outUV;

uniform vec3 center;
uniform vec2 size;
uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform mat4 viewProj;

void main()
{
    vec3 pos = center + cameraRight * inPos.x * size.x + cameraUp * inPos.y * size.y;
    gl_Position = viewProj * vec4(pos, 1.0);
    outUV = inUV;
}
