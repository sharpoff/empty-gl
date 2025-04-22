#pragma once

#include <GL/glew.h>
#include <stdint.h>
#include <vector>
#include <glm/glm.hpp>
#include <renderer/texture.h>
#include <renderer/shader.h>
#include <renderer/material.h>

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normals;
    glm::vec3 color;
};

class Engine;
class Renderer;

class Mesh
{
public:
    friend Engine;
    friend Renderer;
public:
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material);
    void setVertices(std::vector<Vertex> vertices);
    void setIndices(std::vector<uint32_t> indices);
    void setVertexPointers();
    void genBuffers();
    void setBuffersData();

    void setup();
private:
    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;
protected:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    Material material;
};
