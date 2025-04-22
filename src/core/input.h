#pragma once

#include <GLFW/glfw3.h>
#include <map>
#include <vector>

class Input
{
public:
    Input(Input &) = delete;
    void operator=(const Input &) = delete;

    static Input *getSingleton();

    void registerKeys(std::vector<int> keys);
    void update();
    bool isKeyPressed(int key);

    void setCallbacks(GLFWwindow *window);

    bool isMouseLeftPressed();
    bool isMouseRightPressed();

    bool mouseLeftPressed = false;
    bool mouseRightPressed = false;
private:
    Input() = default;
    ~Input();

    void setKey(int key, bool isDown);
    static void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    std::map<int, bool> currentKeyState;
    std::map<int, bool> previousKeyState;

    static Input *singletonInstance;
};
