#include <render/resource_manager.h>
#include <render/renderer.h>

std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Model> ResourceManager::models;
std::map<std::string, Font> ResourceManager::fonts;

Shader *ResourceManager::loadShader(std::string name, std::string vertexPath, std::string fragmentPath)
{
    vertexPath = std::filesystem::path("res/shaders") / vertexPath;
    fragmentPath = std::filesystem::path("res/shaders") / fragmentPath;

    if (ResourceManager::shaders.find(name) != ResourceManager::shaders.end())
        return &ResourceManager::shaders[name];

    // TODO: add support for different shaders, not only vertex and fragment
    Shader shader;
    shader.compile(vertexPath, fragmentPath);
    if (!shader.isLoaded()) {
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
    path = std::filesystem::path("res/textures") / path;
    if (ResourceManager::textures.find(name) != ResourceManager::textures.end())
        return &ResourceManager::textures[name];

    Texture texture;
    texture.loadFromFile(path);
    if (!texture.isLoaded())
        return nullptr;

    ResourceManager::textures[name] = texture;

    Logger::print(LOG_INFO, "Successfully loaded texture ", path);

    return &ResourceManager::textures[name];
}

Texture *ResourceManager::getTexture(std::string name)
{
    if (ResourceManager::textures.find(name) != ResourceManager::textures.end()) {
        return &ResourceManager::textures[name];
    }
    else {
        Logger::print(LOG_WARNING, "Couldn't find texture ", name);
        return nullptr;
    }
}

Model *ResourceManager::loadModel(std::string name, std::string path, Renderer &renderer)
{
    path = std::filesystem::path("res/models") / path;
    if (ResourceManager::models.find(name) != ResourceManager::models.end())
        return &ResourceManager::models[name];

    Model model;
    model.loadFromFile(path, renderer.vertices, renderer.indices);
    if (!model.isLoaded())
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
    else {
        Logger::print(LOG_WARNING, "Couldn't find model ", name);
        return nullptr;
    }
}

Font *ResourceManager::loadFont(std::string name, std::string path, unsigned int fontSize)
{
    path = std::filesystem::path("res/fonts") / path;
    if (ResourceManager::fonts.find(name) != ResourceManager::fonts.end())
        return &ResourceManager::fonts[name];

    Font font;
    font.loadFromFile(path, fontSize);
    if (!font.isLoaded())
        return nullptr;

    ResourceManager::fonts[name] = font;

    Logger::print(LOG_INFO, "Successfully loaded font ", path);

    return &ResourceManager::fonts[name];
}

Font *ResourceManager::getFont(std::string name)
{
    if (ResourceManager::fonts.find(name) != ResourceManager::fonts.end()) {
        return &ResourceManager::fonts[name];
    }
    else {
        Logger::print(LOG_WARNING, "Couldn't find font ", name);
        return nullptr;
    }
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
