#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include <core/logger.h>

class Shader
{
public:
    uint32_t ID;

    bool compile(std::string vertexPath, std::string fragmentPath);
    Shader use();

    void setInt(std::string name, int value);
    void setVector(std::string name, glm::vec2 &value);
    void setVector(std::string name, glm::vec3 &value);
    void setVector(std::string name, glm::vec4 &value);
    void setMatrix(std::string name, glm::mat2 &value);
    void setMatrix(std::string name, glm::mat3 &value);
    void setMatrix(std::string name, glm::mat4 &value);
    void destroy();
private:
    bool checkForErrors(unsigned int shader, std::string type);
};
