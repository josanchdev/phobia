#include "Shader.h"
#include "utils/FileLoader.h"
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = FileLoader::loadTextFile(vertexPath);
    std::string fragmentCode = FileLoader::loadTextFile(fragmentPath);
    
    GLuint vertex = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragment = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
    
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    checkCompileErrors(program, "PROGRAM");
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(program);
}

void Shader::use() const {
    glUseProgram(program);
}

GLuint Shader::compileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "Shader compilation error (" << type << "):\n" << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "Program linking error:\n" << infoLog << std::endl;
        }
    }
}

void Shader::setMat4(const char* name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, value);
}

void Shader::setVec3(const char* name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program, name), x, y, z);
}

void Shader::setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(program, name), value);
}