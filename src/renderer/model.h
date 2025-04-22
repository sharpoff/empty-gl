#pragma once

#include <iostream>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <algorithm>
#include <map>
#include <core/transform.h>

#include <renderer/mesh.h>
#include <core/math.h>

class Model
{
public:
    friend Renderer;

    bool isLoaded();
    void loadFromFile(std::string path, bool isFlip = false);
    void processNode(const aiScene *aScene, const aiNode *aNode, std::string directory);
    Mesh processMesh(const aiScene *aScene, const aiMesh *aMesh, std::string directory);

    mat4 modelMatrix = mat4(1.0);
    Transform transform;
private:
    mat4 convertMatrix4(const aiMatrix4x4 &m);

    std::vector<Mesh> meshes;
    std::map<std::string, Texture> loadedTextures;
    bool loaded;
};
