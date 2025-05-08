#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <render/texture.h>

bool Texture::loadFromFile(std::string path, GLenum target, GLint min, GLint mag, GLint wrap_s, GLint wrap_t)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nComponents, 0);
    if (!data) {
        return false;
    }
    this->width = width;
    this->height = height;

    GLenum format = GL_RGB;
    if (nComponents == 1)
        format = GL_RED;
    else if (nComponents == 3)
        format = GL_RGB;
    else if (nComponents == 4)
        format = GL_RGBA;


    glBindTexture(format, ID);
    glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(format);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
    stbi_image_free(data);

    return true;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::destroy()
{
    glDeleteTextures(1, &ID);
}
