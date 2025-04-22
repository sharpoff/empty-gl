#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormals;
layout (location = 3) in vec3 inColor;

uniform mat4 model, view, projection;

out vec2 texUV;
out vec3 normals;
out vec3 color;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
    texUV = inUV;
    normals = inNormals;
    color = inColor;
}
