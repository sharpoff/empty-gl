#include <render/renderer.h>

void Renderer::init(GLFWwindow *window)
{
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // quad primitive
    quadIndexOffset = indices.size();
    quadVertexOffset = vertices.size();

    vertices.push_back({{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
    vertices.push_back({{-1.0f, 1.0f, 0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}});
    vertices.push_back({{1.0f, 1.0f, 0.0f},   {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}});
    vertices.push_back({{1.0f, -1.0f, 0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});

    std::vector<uint32_t> newIndices = {
        0, 1, 2,
        2, 3, 0,
    };

    for (auto &newIndex : newIndices) {
        newIndex += quadVertexOffset;
        indices.push_back(newIndex);
    }
    quadIndexCount = newIndices.size();

    initGui(window);
}

void Renderer::renderMesh(Shader *shader, Mesh mesh)
{
    if (!shader) {
        Logger::print(LOG_WARNING, "Failed to render mesh.");
    }

    glActiveTexture(GL_TEXTURE0);
    mesh.material.diffuse.bind();
    shader->setInt("material.diffuse", 0);

    glActiveTexture(GL_TEXTURE1);
    mesh.material.specular.bind();
    shader->setInt("material.specular", 1);

    glActiveTexture(GL_TEXTURE2);
    mesh.material.emissive.bind();
    shader->setInt("material.emissive", 2);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (void*)(mesh.indexOffset * sizeof(GLuint)));
}

void Renderer::renderModel(Shader *shader, Model *model)
{
    if (!shader || !model) {
        Logger::print(LOG_WARNING, "Failed to render model.");
        return;
    }

    shader->use();
    shader->setMatrix("model", model->modelMatrix);

    for (unsigned int i = 0; i < model->meshes.size(); i++)
        renderMesh(shader, model->meshes[i]);
}

void Renderer::renderText(Shader *shader, Font *font, std::string text, vec2 position, float scale, vec3 color)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!shader || !font) {
        Logger::print(LOG_WARNING, "Failed to render text.");
        return;
    }

    shader->use();
    shader->setVector("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    std::string::const_iterator it;
    for (it = text.begin(); it != text.end(); it++) {
        Character ch = font->characters[*it];

        glm::vec2 pos;
        pos.x = position.x + ch.bearing.x * scale;
        pos.y = position.y - (ch.size.y - ch.bearing.y) * scale;

        glm::vec2 size;
        size.x = ch.size.x * scale;
        size.y = ch.size.y * scale;

        // change quad
        std::vector<Vertex> vert = {
            {{pos.x, pos.y + size.y, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
            {{pos.x, pos.y, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
            {{pos.x + size.x, pos.y, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
            {{pos.x + size.x, pos.y + size.y, 0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, quadVertexOffset, vert.size() * sizeof(Vertex), vert.data());

        // draw quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glDrawElements(GL_TRIANGLES, quadIndexCount, GL_UNSIGNED_INT, (void*)(quadIndexOffset * sizeof(GLuint)));

        position.x += (ch.advance >> 6) * scale;
    }
}

void Renderer::renderBillboard(Shader *shader, mat4 view, mat4 projection, vec3 position, Texture *texture, vec3 color, vec2 size)
{
    glDisable(GL_CULL_FACE);
    glBindVertexArray(VAO);

    vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);
    mat4 viewProj = projection * view;

    shader->use();
    shader->setMatrix("viewProj", viewProj);
    shader->setVector("cameraRight", cameraRight);
    shader->setVector("cameraUp", cameraUp);
    shader->setVector("center", position);
    shader->setVector("color", color);
    shader->setVector("size", size);
    if (texture) {
        shader->setInt("useTexture", true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->ID);
    }

    // change vertices
    std::vector<Vertex> vert = {
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-1.0f, 1.0f, 0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f},   {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, quadVertexOffset, vert.size() * sizeof(Vertex), vert.data());

    // draw quad
    glDrawElements(GL_TRIANGLES, quadIndexCount, GL_UNSIGNED_INT, (void*)(quadIndexOffset * sizeof(GLuint)));
}

void Renderer::initGui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void Renderer::setupBuffers()
{
    // create buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind buffers
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    // set vertex attributes
    if (vertices.empty()) {
        Logger::print(LOG_WARNING, "No vertices, can't set attributes!");
        return;
    }
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}
