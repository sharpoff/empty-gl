#ifndef FONT_H_
#define FONT_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <string>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <core/logger.h>

struct Character {
    unsigned int textureID;
    glm::ivec2 size; // size of glyph
    glm::ivec2 bearing; // offset from baseline to the left/top
    unsigned int advance; // offset to advance to next glyph
};

class Font
{
public:
    void loadFromFile(std::string font, unsigned int fontSize);
    bool isLoaded();
    void destroy();

    std::map<char, Character> characters;
private:
    void loadCharacters();

    FT_Library ftLib;
    FT_Face ftFace;
    bool loaded;

};

#endif
