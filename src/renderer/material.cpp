#include <renderer/material.h>

Material::Material(Texture ambient, Texture diffuse, Texture specular)
    : ambient(ambient), diffuse(diffuse), specular(specular)
{}

Material::Material()
{
}
