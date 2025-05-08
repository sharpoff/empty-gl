#include <render/resource_manager.h>
#include <render/renderer.h>

std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Model> ResourceManager::models;
std::map<std::string, Font> ResourceManager::fonts;

std::filesystem::path assetsPath = "assets/";
std::filesystem::path shadersPath = assetsPath / "shaders";
std::filesystem::path modelsPath = assetsPath / "models";
std::filesystem::path texturesPath = assetsPath / "textures";
std::filesystem::path fontsPath = assetsPath / "fonts";

Shader *ResourceManager::loadShader(std::string name, std::string vertexPath, std::string fragmentPath, bool force)
{
    vertexPath = shadersPath / vertexPath;
    fragmentPath = shadersPath / fragmentPath;

    Logger::print(LOG_INFO, "Loading shader ", name);

    if (!force && ResourceManager::shaders.find(name) != ResourceManager::shaders.end())
        return &ResourceManager::shaders[name];

    // TODO: add support for different shaders, not only vertex and fragment
    Shader shader;
    if (!shader.compile(vertexPath, fragmentPath)) {
        Logger::print(LOG_ERROR, "Failed to load shader '", name, "'");
        return nullptr;
    }

    ResourceManager::shaders[name] = shader;
    Logger::print(LOG_INFO, "Successfully loaded shader ", name);

    return &ResourceManager::shaders[name];
}

Shader *ResourceManager::getShader(std::string name)
{
    return &ResourceManager::shaders[name];
}

Texture *ResourceManager::loadTexture(std::string name, std::string path)
{
    path = texturesPath / path;
    if (ResourceManager::textures.find(name) != ResourceManager::textures.end())
        return &ResourceManager::textures[name];

    Texture texture;
    if (!texture.loadFromFile(path)) {
        Logger::print(LOG_WARNING, "Texture failed to load at path: ", path);
        return nullptr;
    }

    ResourceManager::textures[name] = texture;
    Logger::print(LOG_INFO, "Successfully loaded texture ", path);

    return &ResourceManager::textures[name];
}

Texture *ResourceManager::getTexture(std::string name)
{
    if (ResourceManager::textures.find(name) != ResourceManager::textures.end())
        return &ResourceManager::textures[name];

    Logger::print(LOG_WARNING, "Failed to find texture ", name);
    return nullptr;
}

Model *ResourceManager::loadModel(std::string name, std::string path, Renderer &renderer)
{
    path = modelsPath / path;
    if (ResourceManager::models.find(name) != ResourceManager::models.end())
        return &ResourceManager::models[name];

    Model model;
    if (!model.loadFromFile(path, renderer.vertices, renderer.indices))
        return nullptr;

    ResourceManager::models[name] = model;
    Logger::print(LOG_INFO, "Successfully loaded model ", path);

    return &ResourceManager::models[name];
}

Model *ResourceManager::getModel(std::string name)
{
    if (ResourceManager::models.find(name) != ResourceManager::models.end()) {
        return &ResourceManager::models[name];
    }

    Logger::print(LOG_WARNING, "Failed to find model ", name);
    return nullptr;
}

Font *ResourceManager::loadFont(std::string name, std::string path, unsigned int fontSize)
{
    path = fontsPath / path;
    if (ResourceManager::fonts.find(name) != ResourceManager::fonts.end())
        return &ResourceManager::fonts[name];

    Font font;
    if (!font.loadFromFile(path, fontSize)) {
        Logger::print(LOG_WARNING, "Failed to load font");
        return nullptr;
    }

    ResourceManager::fonts[name] = font;
    Logger::print(LOG_INFO, "Successfully loaded font ", path);

    return &ResourceManager::fonts[name];
}

Font *ResourceManager::getFont(std::string name)
{
    if (ResourceManager::fonts.find(name) != ResourceManager::fonts.end())
        return &ResourceManager::fonts[name];

    Logger::print(LOG_WARNING, "Failed to find font ", name);
    return nullptr;
}

void ResourceManager::clear()
{
    for (auto it : shaders) {
        it.second.destroy();
    }

    for (auto it : textures) {
        it.second.destroy();
    }
}
