#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <renderer/resource_manager.h>
#include <core/transform.h>
#include <renderer/mesh.h>
#include <renderer/font.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class Renderer
{
public:
    void init(GLFWwindow *window);
    void update();

    void renderMesh(Shader *shader, Mesh mesh);
    void renderModel(Shader *shader, Model *model);
    void renderText(Shader *shader, Font *font, std::string text, glm::vec2 position, float scale, glm::vec3 color);

    void initGui(GLFWwindow *window);
    void renderImGui();

    double fpsCount{0.0}, msCount{0.0};
    uint32_t numFrames{0};

    bool multisampling = true;
    bool wireframe = false;
};
