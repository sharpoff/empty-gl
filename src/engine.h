#pragma once

#include <iostream>
#include <string>
#include <map>
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

    bool isInitialized = false;
};
