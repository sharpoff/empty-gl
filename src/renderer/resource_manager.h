#pragma once

#include <map>
#include <string>
#include <renderer/shader.h>
#include <renderer/texture.h>
#include <renderer/model.h>
#include <renderer/font.h>

#include <core/logger.h>

class ResourceManager
{
public:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture> textures;
    static std::map<std::string, Model> models;
    static std::map<std::string, Font> fonts;

    static Shader *loadShader(std::string name, std::string vsPath, std::string fsPath);
    static Shader *getShader(std::string name);
    static Texture *loadTexture(std::string name, std::string path);
    static Texture *getTexture(std::string name);
    static Model *loadModel(std::string name, std::string path, bool isFlip = false);
    static Model *getModel(std::string name);
    static Font *loadFont(std::string name, std::string path, unsigned int fontSize);
    static Font *getFont(std::string name);

    static void clear();
private:
    ResourceManager();
};
