#include <core/transform.h>

void Transform::translate(vec3 position)
{
    mTranslation += position;
}

void Transform::rotate(float angle, vec3 axis)
{
    mRotation *= angleAxis(angle, axis);
}

void Transform::rotate(quat rotation)
{
    mRotation *= rotation;
}

void Transform::scale(vec3 scale)
{
    mScale *= scale;
}

void Transform::setTranslation(vec3 position)
{
    mTranslation = position;
}

void Transform::setRotation(float angle, vec3 axis)
{
    mRotation = angleAxis(angle, axis);
}

void Transform::setRotation(quat rotation)
{
    mRotation = rotation;
}

void Transform::setScale(vec3 scale)
{
    mScale = scale;
}

vec3 Transform::getTranslation()
{
    return mTranslation;
}

quat Transform::getRotation()
{
    return mRotation;
}

vec3 Transform::getScale()
{
    return mScale;
}

mat4 Transform::getModelMatrix()
{
    mat4 model = mat4(1.0);
    model = glm::translate(model, mTranslation);
    model = model * glm::toMat4(mRotation);
    model = glm::scale(model, mScale);

    return model;
}
