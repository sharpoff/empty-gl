#include <renderer/shader.h>

void Shader::compile(std::string vertexPath, std::string fragmentPath)
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
        Logger::print("Cannot read shader file: " + std::string(e.what()), LOG_WARNING);
        loaded = false;
        return;
    }

    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);
    checkForErrors(vertex, "vertex");

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);
    checkForErrors(fragment, "fragment");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkForErrors(ID, "program");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    loaded = true;
}

bool Shader::isLoaded()
{
    return loaded;
}

Shader Shader::use()
{
    if (loaded)
        glUseProgram(ID);
    else
        Logger::print("Shader that is not loaded cannot be used.", LOG_WARNING);

    return *this;
}

void Shader::destroy()
{
    if (loaded)
        glDeleteProgram(ID);
}

void Shader::setInt(std::string name, int value)
{
    if (loaded)
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVector(std::string name, glm::vec2 &value)
{
    if (loaded)
        glUniform2f(glGetUniformLocation(ID, name.c_str()), value[0], value[1]);
}

void Shader::setVector(std::string name, glm::vec3 &value)
{
    if (loaded)
        glUniform3f(glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2]);
}

void Shader::setVector(std::string name, glm::vec4 &value)
{
    if (loaded)
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2], value[3]);
}

void Shader::setMatrix(std::string name, glm::mat2 &value)
{
    if (loaded)
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMatrix(std::string name, glm::mat3 &value)
{
    if (loaded)
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMatrix(std::string name, glm::mat4 &value)
{
    if (loaded)
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::checkForErrors(unsigned int shader, std::string type)
{
    int success;
    char infoBuff[512];
    if (type == "program") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoBuff);
            Logger::print("Shader program compile error: " + std::string(infoBuff), LOG_ERROR);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoBuff);
            Logger::print(type + " shader program compile error: " + std::string(infoBuff), LOG_ERROR);
        }
    }
}
