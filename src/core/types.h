#pragma once

#include <render/texture.h>
#include <core/math.h>

struct Material
{
    Texture ambient;
    Texture diffuse;
    Texture specular;
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
    vec3 pos;
    vec3 color;
};
