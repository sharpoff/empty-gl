#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <render/resource_manager.h>
#include <core/transform.h>
#include <render/font.h>
#include <core/types.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class Renderer
{
public:
    friend class ResourceManager;

    void init(GLFWwindow *window);
    void update();

    void renderModel(Shader *shader, Model *model);
    void renderText(Shader *shader, Font *font, std::string text, glm::vec2 position, float scale, glm::vec3 color);
    void renderImGui();

    void setupBuffers();
private:
    void renderMesh(Shader *shader, Mesh mesh);
    void initGui(GLFWwindow *window);

    double fpsCount{0.0}, msCount{0.0};
    uint32_t numFrames{0};

    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    bool multisampling = true;
    bool wireframe = false;
};
