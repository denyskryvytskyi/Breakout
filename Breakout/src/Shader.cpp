#include "Shader.h"

#include <iostream>

Shader& Shader::Use()
{
    glUseProgram(m_id);
    return *this;
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource /* = nullptr*/)
{
    unsigned int vShader, fShader, gShader;
    // vertex Shader
    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertexSource, nullptr);
    glCompileShader(vShader);
    checkCompileErrors(vShader, "VERTEX");
    // fragment Shader
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragmentSource, nullptr);
    glCompileShader(fShader);
    checkCompileErrors(fShader, "FRAGMENT");
    // geometry Shader (optional)
    if (geometrySource != nullptr)
    {
        gShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(gShader, 1, &fragmentSource, nullptr);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }
    // shader program
    m_id = glCreateProgram();
    glAttachShader(m_id, vShader);
    glAttachShader(m_id, fShader);
    if (geometrySource != nullptr)
    {
        glAttachShader(m_id, gShader);
    }
    glLinkProgram(m_id);
    checkCompileErrors(m_id, "PROGRAM");

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    if (geometrySource != nullptr)
    {
        glDeleteShader(gShader);
    }
}

void Shader::SetFloat(const char* name, float value, bool useShader)
{
    if (useShader)
    {
        Use();
    }
    glUniform1f(glGetUniformLocation(m_id, name), value);
}

void Shader::SetInteger(const char* name, int value, bool useShader)
{
    if (useShader)
    {
        Use();
    }
    glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::SetVector2f(const char* name, float x, float y, bool useShader)
{
    if (useShader)
    {
        Use();
    }
    glUniform2f(glGetUniformLocation(m_id, name), x, y);
}

void Shader::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader)
    {
        Use();
    }
    glUniform2f(glGetUniformLocation(m_id, name), value.x, value.y);
}
void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
    if (useShader)
    {
        Use();
    }
    glUniform3f(glGetUniformLocation(m_id, name), x, y, z);
}
void Shader::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader)
    {
        Use();
    }
    glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
    {
        Use();
    }
    glUniform4f(glGetUniformLocation(m_id, name), x, y, z, w);
}
void Shader::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
    if (useShader)
    {
        Use();
    }
    glUniform4f(glGetUniformLocation(m_id, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader)
    {
        Use();
    }
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, false, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- -------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- -------------------------------------------------- -- " << std::endl;
        }
    }
}
