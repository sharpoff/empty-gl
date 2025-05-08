#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include <core/logger.h>

class Texture
{
public:
    bool loadFromFile(std::string path, GLenum target = GL_TEXTURE_2D, GLint min = GL_LINEAR, GLint mag = GL_LINEAR, GLint wrap_s = GL_REPEAT, GLint wrap_t = GL_REPEAT);

    void bind();
    void destroy();

    GLuint ID;
    uint32_t width, height;
    std::string type;
};
