#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <core/input.h>
#include <core/camera.h>
#include <core/logger.h>
#include <core/math.h>

#include <render/renderer.h>
#include <render/model.h>

class Engine
{
public:
    Engine() = default;
    ~Engine() = default;

    void init(std::string title, float width, float height);
    void run();
private:
    void render();
    void renderImGui();
    void update();
    void destroy();
    void updateInput(double dt);

    void loadResources();
    void loadShaders(bool reload = false);

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    static void mouseCallback(GLFWwindow *window, double xPos, double yPos);

    GLFWwindow *window;
    int width, height;

    Renderer renderer;
    Camera camera;

    Light light;

    // cacodemon(DOOM) billboard
    struct Cacodemon {
        vec3 pos = vec3(0.0, 2.0, 0.0);
        vec3 color = vec3(1.0);
        vec2 size = vec2(0.2);
    } cacodemon;

    bool isInitialized = false;

    bool multisampling = true;
    bool wireframe = false;

    double fpsCount{0.0}, msCount{0.0};
    uint32_t numFrames{0};
};
