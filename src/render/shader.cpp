#include <render/shader.h>
#include <fstream>
#include <sstream>

bool Shader::compile(std::string vertexPath, std::string fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    try {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        std::stringstream vertexStream, fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();
        vertexFile.close();
        fragmentFile.close();
        // convert stream into string
        vertexCode   = vertexStream.str();
        fragmentCode = fragmentStream.str();
    } catch(std::ifstream::failure &e) {
        Logger::print(LOG_WARNING, "Cannot read shader file: ", std::string(e.what()));
        return false;
    }

    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);
    if (!checkForErrors(vertex, "vertex"))
        return false;

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);
    if (!checkForErrors(fragment, "fragment"))
        return false;

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    if (!checkForErrors(ID, "program"))
        return false;

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

Shader Shader::use()
{
    glUseProgram(ID);

    return *this;
}

void Shader::destroy()
{
    glDeleteProgram(ID);
}

void Shader::setInt(std::string name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVector(std::string name, glm::vec2 &value)
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value[0], value[1]);
}

void Shader::setVector(std::string name, glm::vec3 &value)
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2]);
}

void Shader::setVector(std::string name, glm::vec4 &value)
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2], value[3]);
}

void Shader::setMatrix(std::string name, glm::mat2 &value)
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMatrix(std::string name, glm::mat3 &value)
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMatrix(std::string name, glm::mat4 &value)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

bool Shader::checkForErrors(unsigned int shader, std::string type)
{
    int success;
    char infoBuff[512];
    if (type == "program") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoBuff);
            Logger::print(LOG_ERROR, "Shader program compile error: ", std::string(infoBuff));
            return false;
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoBuff);
            Logger::print(LOG_ERROR, type + " shader program compile error: ", std::string(infoBuff));
            return false;
        }
    }

    return true;
}
