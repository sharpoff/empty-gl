#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

out vec2 outUV;
out vec3 outNormal;
out vec3 fragPos;
out vec3 viewPos;

void main()
{
    vec4 world = model * vec4(inPos, 1.0);
    gl_Position = projection * view * world;
    outUV = inUV;
    outNormal = inNormal;

    fragPos = vec3(world);
    viewPos = cameraPos;
}
