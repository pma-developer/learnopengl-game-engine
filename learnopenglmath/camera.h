
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "global_consts.h"

class Camera
{


public: 
    Camera(float fovY, float aspectRatio, float nearClip, float farClip);
    Camera(float aspectRatio);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void rotate(const glm::vec2& rotationYX);
    void moveLocal(const glm::vec3& inputVelocity);

private:
    glm::vec3 cameraPosition;
    glm::vec2 cameraOrientation;
    float fovY;
    float aspectRatio;
    float nearClip;
    float farClip;
};