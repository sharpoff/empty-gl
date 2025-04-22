#include <renderer/model.h>
#include <assimp/postprocess.h>

bool Model::isLoaded()
{
    return loaded;
}

void Model::loadFromFile(std::string path, bool isFlip)
{
    Assimp::Importer importer;

    unsigned int pFlags = aiProcess_Triangulate | aiProcess_OptimizeMeshes;
    if (isFlip)
        pFlags |= aiProcess_FlipUVs;
    const aiScene *aScene = importer.ReadFile(path, pFlags);

    if (aScene == nullptr) {
        Logger::print("Failed to load model " + path + ": " + importer.GetErrorString(), LOG_WARNING);
        return;
    }

    std::string directory = std::filesystem::path(path).parent_path();
    processNode(aScene, aScene->mRootNode, directory);

    loaded = true;
}

void Model::processNode(const aiScene *aScene, const aiNode *aNode, std::string directory)
{
    modelMatrix *= convertMatrix4(aNode->mTransformation);

    for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
        const aiMesh *aMesh = aScene->mMeshes[aNode->mMeshes[i]];
        meshes.push_back(processMesh(aScene, aMesh, directory));
    }

    for (unsigned int i = 0 ; i < aNode->mNumChildren; i++) {
        processNode(aScene, aNode->mChildren[i], directory);
    }
}

Mesh Model::processMesh(const aiScene *aiScene, const aiMesh *aiMesh, std::string directory)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    Material material;
    // default material
//    Texture default_tex;
//    default_tex.loadFromFile();
//    material.ambient = default_tex;
//    material.diffuse = default_tex;
//    material.specular = default_tex;

    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position.x = aiMesh->mVertices[i].x;
        vertex.position.y = aiMesh->mVertices[i].y;
        vertex.position.z = aiMesh->mVertices[i].z;

        if (aiMesh->HasNormals()) {
            vertex.normals.x = aiMesh->mNormals[i].x;
            vertex.normals.y = aiMesh->mNormals[i].y;
            vertex.normals.z = aiMesh->mNormals[i].z;
        } else {
            vertex.normals = glm::vec3(0);
        }

        if (aiMesh->HasTextureCoords(0)) {
            vertex.uv.x = aiMesh->mTextureCoords[0][i].x;
            vertex.uv.y = aiMesh->mTextureCoords[0][i].y;
        } else {
            vertex.uv = glm::vec2(0);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
        aiFace aFace = aiMesh->mFaces[i];
        for (unsigned int j = 0; j < aFace.mNumIndices; j++) {
            indices.push_back(aFace.mIndices[j]);
        }
    }

    if (aiMesh->mMaterialIndex >= 0) {
        const aiMaterial *aMaterial = aiScene->mMaterials[aiMesh->mMaterialIndex];
        aiString path;

        // TODO: maybe remove duplicate code
        if (aMaterial->GetTextureCount(aiTextureType_AMBIENT) > 0 && aMaterial->GetTexture(aiTextureType_AMBIENT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
            Texture amb;
            if (loadedTextures.find(path.data) == loadedTextures.end()) {
                std::string fullpath = directory + '/' + path.data;
                Logger::print("loading: " + fullpath, LOG_INFO);
                amb.loadFromFile(fullpath);
                material.ambient = amb;
                loadedTextures[path.data] = amb;
            } else {
                material.ambient = loadedTextures[path.data];
            }
        }

        if (aMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0 && aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
            Texture diff;
            if (loadedTextures.find(path.data) == loadedTextures.end()) {
                std::string fullpath = directory + '/' + path.data;
                Logger::print("loading: " + fullpath, LOG_INFO);
                diff.loadFromFile(fullpath);
                material.diffuse = diff;
                loadedTextures[path.data] = diff;
            } else {
                material.diffuse = loadedTextures[path.data];
            }
        }

        if (aMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0 && aMaterial->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
            Texture spec;
            if (loadedTextures.find(path.data) == loadedTextures.end()) {
                std::string fullpath = directory + '/' + path.data;
                Logger::print("loading: " + fullpath, LOG_INFO);
                spec.loadFromFile(fullpath);
                material.specular = spec;
                loadedTextures[path.data] = spec;
            } else {
                material.specular = loadedTextures[path.data];
            }
        }
    }

    return Mesh(vertices, indices, material);
}

mat4 Model::convertMatrix4(const aiMatrix4x4 &m)
{
    glm::mat4 mat;
    mat[0][0] = m.a1; mat[1][0] = m.a2; mat[2][0] = m.a3; mat[3][0] = m.a4;
    mat[0][1] = m.b1; mat[1][1] = m.b2; mat[2][1] = m.b3; mat[3][1] = m.b4;
    mat[0][2] = m.c1; mat[1][2] = m.c2; mat[2][2] = m.c3; mat[3][2] = m.c4;
    mat[0][3] = m.d1; mat[1][3] = m.d2; mat[2][3] = m.d3; mat[3][3] = m.d4;

    return mat;
}
