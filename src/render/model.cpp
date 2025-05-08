#include <render/model.h>
#include <assimp/postprocess.h>

bool Model::loadFromFile(std::filesystem::path path, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices)
{
    Assimp::Importer importer;

    unsigned int pFlags = aiProcess_Triangulate | aiProcess_OptimizeMeshes;
    const aiScene *aScene = importer.ReadFile(path, pFlags);

    if (aScene == nullptr) {
        Logger::print(LOG_WARNING, "Failed to load model ", path, ": ", importer.GetErrorString());
        return false;
    }

    std::string directory = std::filesystem::path(path).parent_path();
    processNode(aScene, aScene->mRootNode, directory, vertices, indices);

    return true;
}

void Model::processNode(const aiScene *aScene, const aiNode *aNode, std::filesystem::path directory, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices)
{
    modelMatrix *= convertMatrix4(aNode->mTransformation);

    for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
        const aiMesh *aMesh = aScene->mMeshes[aNode->mMeshes[i]];
        meshes.push_back(processMesh(aScene, aMesh, directory, vertices, indices));
    }

    for (unsigned int i = 0 ; i < aNode->mNumChildren; i++) {
        processNode(aScene, aNode->mChildren[i], directory, vertices, indices);
    }
}

Mesh Model::processMesh(const aiScene *aiScene, const aiMesh *aiMesh, std::filesystem::path directory, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices)
{
    Mesh mesh = {};

    uint32_t indexOffset = indices.size();
    uint32_t vertexOffset = vertices.size();
    uint32_t indexCount = 0;

    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.pos.x = aiMesh->mVertices[i].x;
        vertex.pos.y = aiMesh->mVertices[i].y;
        vertex.pos.z = aiMesh->mVertices[i].z;

        if (aiMesh->HasNormals()) {
            vertex.normal.x = aiMesh->mNormals[i].x;
            vertex.normal.y = aiMesh->mNormals[i].y;
            vertex.normal.z = aiMesh->mNormals[i].z;
        } else {
            vertex.normal = glm::vec3(0);
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
            indices.push_back(aFace.mIndices[j] + vertexOffset);
            indexCount++;
        }
    }

    if (aiMesh->mMaterialIndex >= 0) {
        const aiMaterial *aMaterial = aiScene->mMaterials[aiMesh->mMaterialIndex];

        mesh.material.diffuse = loadMaterialTexture(aMaterial, aiTextureType_DIFFUSE, 0, directory);
        mesh.material.specular = loadMaterialTexture(aMaterial, aiTextureType_SPECULAR, 0, directory);
        mesh.material.emissive = loadMaterialTexture(aMaterial, aiTextureType_EMISSIVE, 0, directory);
    }

    mesh.indexOffset = indexOffset;
    mesh.indexCount = indexCount;

    return mesh;
}

Texture Model::loadMaterialTexture(const aiMaterial *aMaterial, aiTextureType type, unsigned int index, std::filesystem::path directory)
{
    aiString texturePath;

    Texture texture;
    if (aMaterial->Get(AI_MATKEY_TEXTURE(type, index), texturePath) == AI_SUCCESS) {
        std::filesystem::path fullTexturePath = directory / std::filesystem::path(texturePath.data);

        if (textures.find(fullTexturePath) == textures.end()) {
            Logger::print(LOG_INFO, "Loading texture: ", fullTexturePath);

            texture.loadFromFile(fullTexturePath);
            textures[fullTexturePath] = texture;
        } else {
            texture = textures[fullTexturePath];
        }
    }

    return texture;
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
