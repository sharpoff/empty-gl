#include <core/input.h>

Input *Input::singletonInstance = nullptr;

Input::~Input()
{
    if (singletonInstance)
        delete singletonInstance;
}

Input *Input::getSingleton()
{
    if (!singletonInstance)
        singletonInstance = new Input();

    return singletonInstance;
}

void Input::registerKeys(std::vector<int> keys) {
    for (const int &key : keys) {
        currentKeyState[key] = false;
    }
}

void Input::update()
{
    previousKeyState = currentKeyState;
}

bool Input::isKeyPressed(int key)
{
    if (currentKeyState.find(key) != currentKeyState.end() && previousKeyState.find(key) != previousKeyState.end()) {
        if (currentKeyState[key] == true || previousKeyState[key] == true) {
            return true;
        }
    }

    return false;
}

void Input::setKey(int key, bool state)
{
    if (currentKeyState.find(key) != currentKeyState.end()) {
        currentKeyState[key] = state;
    }
}

void Input::setCallbacks(GLFWwindow *window)
{
    glfwSetKeyCallback(window, Input::keyboardCallback);
    glfwSetMouseButtonCallback(window, Input::mouseButtonCallback);
}

void Input::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    Input::getSingleton()->setKey(key, action == GLFW_PRESS || action == GLFW_REPEAT);
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        Input::getSingleton()->mouseLeftPressed = true;
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        Input::getSingleton()->mouseLeftPressed = false;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        Input::getSingleton()->mouseRightPressed = true;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        Input::getSingleton()->mouseRightPressed = false;
}

bool Input::isMouseLeftPressed()
{
    return mouseLeftPressed;
}

bool Input::isMouseRightPressed()
{
    return mouseRightPressed;
}
