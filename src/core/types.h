#pragma once

#include <render/texture.h>
#include <core/math.h>

struct Material
{
    Texture diffuse;
    Texture specular;
    Texture emissive;
};

struct Vertex {
    vec3 pos;
    vec2 uv;
    vec3 normal;
};

struct Mesh
{
    Material material;
    uint32_t indexOffset;
    uint32_t indexCount;
};

struct Light
{
    vec3 pos = vec3(0.0);
    vec3 color = vec3(1.0);

    vec2 size = vec2(0.2); // billboard size
};
