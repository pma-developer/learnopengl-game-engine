
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    unsigned int programID;
    std::string vShaderPath;
    std::string fShaderPath;

    Shader();
    Shader(std::string vShaderPath, std::string fShaderPath);
    Shader(std::string vShaderPath, std::string fShaderPath, std::function<std::string(std::string)> fShaderPreprocessor);

    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec4(const std::string& name, float v0, float v1, float v2, float v3) const;
    void setVec4(const std::string& name, glm::vec4 v) const;
    void setVec3(const std::string& name, float v0, float v1, float v2) const;
    void setVec3(const std::string& name, glm::vec3 v) const;
    void setVec2(const std::string& name, float v0, float v1) const;
    void setBoolArray(const std::string& name, GLsizei size, bool* value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;

private:
    unsigned int getShaderProgram();
    void getShaderSourcesFromPaths(std::string& vShaderSource, std::string& fShaderSource, std::string vShaderPath, std::string fShaderPath);
    unsigned int getShaderProgramFromSources(std::string vShaderSource, std::string fShaderSource);
    void logErrors(unsigned int id, std::string type);
};
