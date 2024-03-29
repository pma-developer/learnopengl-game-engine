
#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>


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
