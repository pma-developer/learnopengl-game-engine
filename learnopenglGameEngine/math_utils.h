
#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <optional>
#include <iomanip>


float length2(const glm::vec2& v)
{
    return v.x * v.x + v.y * v.y;
}
float length2(const glm::vec3& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}
void logVector(const glm::vec3& v, std::string name)
{
    std::cout << name <<" x:" << v.x << " y: " << v.y << " z: " << v.z << std::endl;
}
void logNonZeroVector(const glm::vec3& v, std::string name)
{
    if(length2(v) > 0)
        std::cout << name << " x:" << v.x << " y: " << v.y << " z: " << v.z << std::endl;
}

template<glm::length_t rows, glm::length_t cols>
void logMatrix(const glm::mat<rows, cols, glm::f32>& matrix, const std::optional<std::string> name = std::nullopt)
{
    std::cout << "Matrix " << name.value_or("") << std::endl;

    int max_width = 0;
    std::ostringstream stream;
    for (glm::length_t i = 0; i < rows; i++) {
        for (glm::length_t j = 0; j < cols; j++) {
            stream.str("");
            stream << std::fixed << std::setprecision(2) << matrix[j][i];
            max_width = std::max(max_width, static_cast<int>(stream.str().find('.')));
        }
    }

    for (glm::length_t i = 0; i < rows; i++)
    {
        for (glm::length_t j = 0; j < cols; j++)
        {
            stream.str("");
            stream << std::fixed << std::setprecision(2) << matrix[j][i];
            std::string num = stream.str();
            int decimal_pos = num.find('.');
            std::cout << std::string(max_width - decimal_pos, ' ') << num << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "-----------" << std::endl;
}