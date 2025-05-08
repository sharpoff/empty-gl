#pragma once

#include <GLFW/glfw3.h>

#include <core/math.h>

class Camera
{
public:
    void update(float deltaTime);
    mat4 getViewMatrix();
    mat4 getRotationMatrix();

    void setPosition(vec3 position);
    void setMovementSpeed(float speed);
    void setRotationSpeed(float speed);

    vec3 &getPosition();

    void updateRotation(double xPos, double yPos);
    void updatePosition(double dt);
private:
    vec3 position = vec3(0.0f);
    vec3 velocity = vec3(0.0f);
    float pitch = 0.0f;
    float yaw = 0.0f;

    float movementSpeed = 1.5;
    float rotationSpeed = 0.01;

    vec2 previousMousePos = vec2(0);
};
