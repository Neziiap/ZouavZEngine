#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include "Maths/Mat4.hpp"
#include "Maths/Vec3.hpp"
#include "Maths/Vec2.hpp"
#include "Maths/Vec4.hpp"
#include "Component/Light.hpp"
#include "GameObject.hpp"
#include "System/Debug.hpp"
#include "System/ResourcesManager.hpp"
#include "Rendering/Shader.hpp"

Shader::Shader(const std::string& _name, const char* _shaderPath)
    : Resource(_name, _shaderPath)
{
    const char* shaderSource = LoadFile(_shaderPath);
    std::string vshader = "#version 330 core\r\n#define COMPILING_VS\r\n" + std::string(shaderSource);
    std::string fshader = "#version 330 core\r\n#define COMPILING_FS\r\n" + std::string(shaderSource);
    
    unsigned int vertexShader = CreateVertexShader(vshader.c_str(), _shaderPath);
    unsigned int fragmentShader = CreateFragmentShader(fshader.c_str(), _shaderPath);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    delete[] shaderSource;
}

unsigned int Shader::CreateVertexShader(const char* _vertexShaderContent, const char* _shaderPath)
{
    const char* vertexShaderSource = _vertexShaderContent;
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    DebugCheck(vertexShader, "VERT ERROR", _shaderPath);

    return vertexShader;
}

unsigned int Shader::CreateFragmentShader(const char* _fragmentShaderContent, const char* _shaderPath)
{
    const char* fragmentShaderSource = _fragmentShaderContent;
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    DebugCheck(fragmentShader, "FRAG ERROR", _shaderPath);

    return fragmentShader;
}

const char* Shader::LoadFile(std::string _pathToFile)
{
    std::ifstream file(_pathToFile.c_str(), std::ios::binary);

    if (file)
    {
        file.seekg(0, file.end);
        int length = (int)file.tellg();
        file.seekg(0, file.beg);

        char* buffer = new char[length + 1];

        file.read(buffer, length);
        buffer[length] = '\0';

        file.close();

        return buffer;
    }

    return nullptr;
}


void Shader::DebugCheck(const int& _shader, const char* _msg, std::string _src)
{
    int success;
    char infoLog[1024];
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(_shader, 1024, NULL, infoLog);
        Debug::LogWarning(std::string(_msg).append(" : ").append(_src).append("\n").append(infoLog).append("\n"));
    }
}


void Shader::Use()
{
    glUseProgram(id);
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::RemoveFromResourcesManager()
{
    ResourcesManager::RemoveResourceShader(name);
}

void Shader::SetBool(const std::string& _name, bool _value) const
{
    glUniform1i(glGetUniformLocation(id, _name.c_str()), (int)_value);
}

void Shader::SetInt(const std::string& _name, int _value) const
{
    glUniform1i(glGetUniformLocation(id, _name.c_str()), _value);
}

void Shader::SetFloat(const std::string& _name, float _value) const
{
    glUniform1f(glGetUniformLocation(id, _name.c_str()), _value);
}

void Shader::SetFloatArray(const std::string& _name, const float* _value, unsigned int _size) const
{
    glUniform1fv(glGetUniformLocation(id, _name.c_str()), _size, _value);
}

void Shader::SetMatrix(const std::string& _name, const Mat4& _mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, _name.c_str()), 1, GL_FALSE, _mat.matrix);
}

void Shader::SetMatrices(const std::string& _name, const float* _mat, unsigned int _size) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, _name.c_str()), _size, GL_FALSE, _mat);
}

void Shader::SetVector2(const std::string& _name, const Vec2& _v) const
{
    glUniform2f(glGetUniformLocation(id, _name.c_str()), _v.x, _v.y);
}

void Shader::SetVector3(const std::string& _name, float _x, float _y, float _z) const
{
    glUniform3f(glGetUniformLocation(id, _name.c_str()), _x, _y, _z);
}

void Shader::SetVector3(const std::string& _name, const Vec3& _v) const
{
    glUniform3f(glGetUniformLocation(id, _name.c_str()), _v.x, _v.y, _v.z);
}

void Shader::SetVector4(const std::string& _name, float _w, float _x, float _y, float _z) const
{
    glUniform4f(glGetUniformLocation(id, _name.c_str()), _x, _y, _z, _w);
}

void Shader::SetVector4(const std::string& _name, const Vec4& _v) const
{
    glUniform4f(glGetUniformLocation(id, _name.c_str()), _v.x, _v.y, _v.z, _v.w);
}

void Shader::SetLight(const std::vector<Light*>& _lights)
{
    Use();
    int i = 0;
    for (const Light* l : _lights)
    {
        if (!l->IsActive())
            continue;
        const std::string index = std::to_string(i++);
        
        SetVector3(("lights[" + index + "].position").c_str(), l->GetGameObject().WorldPosition());
        SetVector3(("lights[" + index + "].ambient").c_str(), l->ambient);
        SetVector3(("lights[" + index + "].diffuse").c_str(), l->diffuse);
        SetVector3(("lights[" + index + "].specular").c_str(), l->specular);
        SetVector3(("lights[" + index + "].direction").c_str(), l->direction);
        SetVector3(("lights[" + index + "].ConstLineQuad").c_str(), l->constLineQuad);
        SetVector2(("lights[" + index + "].cutOffOuterCutOff").c_str(), l->cutOffOuterCutOff);
        SetInt(("lights[" + index + "].type").c_str(), (int)l->type);
    }
    SetInt("nbLight", i);
}