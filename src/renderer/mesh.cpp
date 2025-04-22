#include <renderer/mesh.h>

Mesh::Mesh()
{}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
    this->vertices = vertices;
    this->indices = indices;

    setup();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material)
{
    this->vertices = vertices;
    this->indices = indices;
    this->material = material;

    setup();
}


void Mesh::setVertices(std::vector<Vertex> vertices)
{
    this->vertices = vertices;
}

void Mesh::setIndices(std::vector<uint32_t> indices)
{
    this->indices = indices;
}

void Mesh::setup()
{
    genBuffers();

    setBuffersData();

    setVertexPointers();
}

void Mesh::genBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void Mesh::setBuffersData()
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Vertex), indices.data(), GL_STATIC_DRAW);
}

void Mesh::setVertexPointers()
{
    if (vertices.empty()) {
        Logger::print("Vertices vector is empty, can't set attributes!", LOG_WARNING);
        return;
    }
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normals));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
}
