#ifndef SHADER_H
#define SHADER_H

#include <glew.h> // Include GLEW for OpenGL function pointers
#include <glm.hpp>
#include <string>

class Shader {
public:
    unsigned int ID;

    // Constructor that builds the shader program from 2 different shaders
    Shader(const char* vertexPath, const char* fragmentPath);

    // Use/activate the shader
    void use() const;

    // Utility functions to set uniform variables
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    // Utility function to check and report compile errors
    void checkCompileErrors(unsigned int shader, const std::string& type);
};

#endif // SHADER_H
