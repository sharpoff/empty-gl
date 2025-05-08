#pragma once

#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include <core/transform.h>
#include <core/math.h>
#include <core/types.h>

class Model
{
public:
    friend class Renderer;

    bool loadFromFile(std::filesystem::path path, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices);
    void processNode(const aiScene *aScene, const aiNode *aNode, std::filesystem::path directory, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices);
    Mesh processMesh(const aiScene *aiScene, const aiMesh *aiMesh, std::filesystem::path directory, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices);

    mat4 modelMatrix = mat4(1.0);
    Transform transform;
private:
    Texture loadMaterialTexture(const aiMaterial *aMaterial, aiTextureType type, unsigned int index, std::filesystem::path directory);
    mat4 convertMatrix4(const aiMatrix4x4 &m);

    std::vector<Mesh> meshes;
    std::map<std::string, Texture> textures;
};
