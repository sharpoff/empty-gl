#include <render/font.h>

void Font::destroy()
{
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLib);
}

void Font::loadFromFile(std::string font, unsigned int fontSize)
{
    // Create library
    if (FT_Init_FreeType(&ftLib)) {
        Logger::print(LOG_WARNING, "Failed to initialize FreeType.");
        loaded = false;
        return;
    }

    // Load Font face
    if (FT_New_Face(ftLib, font.c_str(), 0, &ftFace)) {
        Logger::print(LOG_WARNING, "Failed to load font");
        loaded = false;
        return;
    }

    FT_Set_Pixel_Sizes(ftFace, 0, fontSize);

    loadCharacters();
}

void Font::loadCharacters()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    
    for (unsigned char ch = 0; ch < 128; ch++) {
        if (FT_Load_Char(ftFace, ch, FT_LOAD_RENDER)) {
            Logger::print(LOG_WARNING, "Failed to load glyph for char '", ch, "'");
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, ftFace->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Character character = {
            texture,
            glm::ivec2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
            glm::ivec2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
            static_cast<unsigned int>(ftFace->glyph->advance.x)
        };
        characters[ch] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    destroy();

    loaded = true;
}

bool Font::isLoaded()
{
    return loaded;
}
