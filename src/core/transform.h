#pragma once

#include <core/math.h>

class Transform
{
public:
    void translate(vec3 position);
    void rotate(float angle, vec3 axis);
    void rotate(quat rotation);
    void scale(vec3 scale);

    void setTranslation(vec3 position);
    void setRotation(float angle, vec3 axis);
    void setRotation(quat rotation);
    void setScale(vec3 scale);

    vec3 getTranslation();
    quat getRotation();
    vec3 getScale();

    mat4 getModelMatrix();
private:
    vec3 mTranslation = vec3(0.0f);
    quat mRotation = quat(1.0, 0.0, 0.0, 0.0);
    vec3 mScale = vec3(1.0f);
};
