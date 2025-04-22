#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <core/logger.h>

class Shader
{
public:
    uint32_t ID;

    void compile(std::string vertexPath, std::string fragmentPath);
    bool isLoaded();
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
    bool loaded = false;

    bool checkForErrors(unsigned int shader, std::string type);
};
