#include <core/camera.h>
#include <core/input.h>

void Camera::update(float deltaTime)
{
    updatePosition(deltaTime);

    pitch = glm::clamp(pitch, glm::radians(-89.9f), glm::radians(89.9f));

    mat4 rotation = getRotationMatrix();

    float speedBoost = 1.0f;
    if (Input::getSingleton()->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
        speedBoost = 4.0f;
    }

    vec3 finalSpeed = velocity * movementSpeed * speedBoost * deltaTime;

    position += vec3(rotation * vec4(finalSpeed, 0.0f));
}

mat4 Camera::getViewMatrix()
{
    mat4 translation = glm::translate(mat4(1.0f), position);
    mat4 rotation = getRotationMatrix();
    return glm::inverse(translation * rotation);
}

mat4 Camera::getRotationMatrix()
{
    quat pitchRotation = glm::angleAxis(pitch, vec3(1.0f, 0.0f, 0.0f));
    quat yawRotation = glm::angleAxis(yaw, vec3(0.0f, -1.0f, 0.0f));

    return toMat4(yawRotation) * toMat4(pitchRotation);
}

void Camera::setPosition(vec3 position)
{
    this->position = position;
}

void Camera::setMovementSpeed(float speed)
{
    this->movementSpeed = speed;
}

void Camera::setRotationSpeed(float speed)
{
    this->rotationSpeed = speed;
}

vec3 Camera::getPosition()
{
    return position;
}

// called from glfw callback
void Camera::updateRotation(double xPos, double yPos)
{
    if (Input::getSingleton()->isMouseLeftPressed()) {
        double relativeX = xPos - previousMousePos.x;
        double relativeY = yPos - previousMousePos.y;

        yaw += relativeX * rotationSpeed;
        pitch -= relativeY * rotationSpeed;

        previousMousePos.x = xPos;
        previousMousePos.y = yPos;
    } else {
        previousMousePos.x = xPos;
        previousMousePos.y = yPos;
    }
}

void Camera::updatePosition(double dt)
{
    Input *input = Input::getSingleton();

    vec2 cameraMotion = vec2(input->isKeyPressed(GLFW_KEY_W), input->isKeyPressed(GLFW_KEY_D)) - vec2(input->isKeyPressed(GLFW_KEY_S), input->isKeyPressed(GLFW_KEY_A));

    velocity = vec3(cameraMotion.y, 0.0, -cameraMotion.x);
}
