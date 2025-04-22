#include "renderer.h"

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
        Logger::print("Failed to render mesh.", LOG_WARNING);
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

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::renderModel(Shader *shader, Model *model)
{
    if (!shader || !model) {
        Logger::print("Failed to render model.", LOG_WARNING);
        return;
    }

    shader->use();
    shader->setMatrix("model", model->modelMatrix);

    for (unsigned int i = 0; i < model->meshes.size(); i++) {
        renderMesh(shader, model->meshes[i]);
    }
}

void Renderer::renderText(Shader *shader, Font *font, std::string text, glm::vec2 position, float scale, glm::vec3 color)
{
    if (!shader || !font) {
        Logger::print("Failed to render text.", LOG_WARNING);
        return;
    }

    Mesh quad;
    quad.vertices = std::vector<Vertex> {
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{-1.0f, 1.0f, 0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f},   {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
    };
    quad.indices = std::vector<uint32_t> {
        0, 1, 2,
        2, 3, 0,
    };
    quad.setup();

    shader->use();
    shader->setVector("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(quad.VAO);

    std::string::const_iterator it;
    for (it = text.begin(); it != text.end(); it++) {
        Character ch = font->characters[*it];

        glm::vec2 pos;
        pos.x = position.x + ch.bearing.x * scale;
        pos.y = position.y - (ch.size.y - ch.bearing.y) * scale;

        glm::vec2 size;
        size.x = ch.size.x * scale;
        size.y = ch.size.y * scale;

        quad.vertices = std::vector<Vertex> {
            {{pos.x, pos.y + size.y, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
            {{pos.x, pos.y, 0.0f},  {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
            {{pos.x + size.x, pos.y, 0.0f},   {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
            {{pos.x + size.x, pos.y + size.y, 0.0f},  {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        };
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, quad.VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, quad.vertices.size() * sizeof(Vertex), quad.vertices.data());

        glDrawElements(GL_TRIANGLES, quad.indices.size(), GL_UNSIGNED_INT, 0);

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
