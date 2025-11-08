#pragma once
#include <GL/glew.h>
#include <string>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    
    void use() const;
    GLuint getID() const { return program; }
    
    // Uniform setters
    void setMat4(const char* name, const float* value) const;
    void setVec3(const char* name, float x, float y, float z) const;
    void setFloat(const char* name, float value) const;
    void setInt(const char* name, int value) const;
    
private:
    GLuint program;
    GLuint compileShader(const char* source, GLenum type);
    void checkCompileErrors(GLuint shader, const std::string& type);
};