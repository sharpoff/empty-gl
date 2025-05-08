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

    void renderModel(Shader *shader, Model *model);
    void renderText(Shader *shader, Font *font, std::string text, glm::vec2 position, float scale, glm::vec3 color);
    void renderBillboard(Shader *shader, mat4 view, mat4 projection, vec3 position, Texture *texture, vec3 color, vec2 size);

    void setupBuffers();
private:
    void renderMesh(Shader *shader, Mesh mesh);
    void initGui(GLFWwindow *window);

    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;

    // TODO: make this a struct or something
    uint32_t quadIndexOffset;
    uint32_t quadIndexCount;
    uint32_t quadVertexOffset;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};
