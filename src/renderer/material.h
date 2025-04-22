#pragma once

#include <glm/glm.hpp>
#include "texture.h"

class Material
{
public:
    Material(Texture ambient, Texture diffuse, Texture specular);
    Material();

    Texture ambient;
    Texture diffuse;
    Texture specular;
};
