#include <render/renderer.h>

void Renderer::init(GLFWwindow *window)
{
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    initGui(window);
}

void Renderer::update()
{
    // set state based on settings
    if (multisampling) {
        glEnable(GL_MULTISAMPLE);  
    } else {
        glDisable(GL_MULTISAMPLE);  
    }
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // set fps
    float currentTime = glfwGetTime();
    static float lastTime = glfwGetTime();
    float elapsedTime = currentTime - lastTime;
    if (elapsedTime > 0.25) {
        lastTime = currentTime;
        fpsCount = numFrames / elapsedTime;
        msCount = elapsedTime / numFrames * 1000;
        numFrames = 0;
    }
    numFrames++;
}

void Renderer::renderMesh(Shader *shader, Mesh mesh)
{
    if (!shader) {
        Logger::print(LOG_WARNING, "Failed to render mesh.");
    }

    glActiveTexture(GL_TEXTURE0);
    mesh.material.diffuse.bind();
    shader->setInt("material.ambient", 0);

    glActiveTexture(GL_TEXTURE1);
    mesh.material.diffuse.bind();
    shader->setInt("material.diffuse", 1);

    glActiveTexture(GL_TEXTURE2);
    mesh.material.specular.bind();
    shader->setInt("material.specular", 2);

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

    for (unsigned int i = 0; i < model->meshes.size(); i++) {
        renderMesh(shader, model->meshes[i]);
    }
}

void Renderer::renderText(Shader *shader, Font *font, std::string text, vec2 position, float scale, vec3 color)
{
    if (!shader || !font) {
        Logger::print(LOG_WARNING, "Failed to render text.");
        return;
    }

    uint32_t indexOffset = indices.size();
    uint32_t vertexOffset = vertices.size();

    vertices.push_back({{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}});
    vertices.push_back({{-1.0f, 1.0f, 0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}});
    vertices.push_back({{1.0f, 1.0f, 0.0f},   {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}});
    vertices.push_back({{1.0f, -1.0f, 0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}});

    std::vector<uint32_t> newIndices = std::vector<uint32_t> {
        0, 1, 2,
        2, 3, 0,
    };

    for (auto &newIndex : newIndices) {
        newIndex += vertexOffset;
    }
    indices.insert(indices.end(), newIndices.begin(), newIndices.end());

    uint32_t indexCount = newIndices.size();

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

        vertices = std::vector<Vertex> {
            {{pos.x, pos.y + size.y, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{pos.x, pos.y, 0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{pos.x + size.x, pos.y, 0.0f},   {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{pos.x + size.x, pos.y + size.y, 0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        };
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)(indexOffset * sizeof(GLuint)));

        position.x += (ch.advance >> 6) * scale;
    }
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

void Renderer::renderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    // Debugging window
    {
        ImGui::Begin("Debug");
        // ImGuiIO& io = ImGui::GetIO();

        ImGui::Checkbox("Multisampling", &multisampling);
        ImGui::Checkbox("Wireframe", &wireframe);

        ImGui::Text("%.1f FPS (%.1f ms)", fpsCount, msCount);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
        Logger::print(LOG_WARNING, "Vertices vector is empty, can't set attributes!");
        return;
    }
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}
