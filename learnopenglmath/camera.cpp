
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "global_consts.h"
#include "camera.h"


Camera::Camera(float fovY, float aspectRatio, float nearClip, float farClip)
    : fovY(fovY)
    , aspectRatio(aspectRatio)
    , nearClip(nearClip)
    , farClip(farClip)
{
    cameraPosition = glm::vec3(0, 0, 0);
    cameraOrientation = glm::vec2(0, 0);
}

Camera::Camera(float aspectRatio)
    : Camera(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f)
{
}

glm::mat4 Camera::getViewMatrix()
{
    glm::mat4 view = glm::mat4(1.0f);

    view = glm::rotate(view, glm::radians(cameraOrientation.y), AXIS_RIGHT);
    view = glm::rotate(view, glm::radians(cameraOrientation.x), AXIS_UP);
    view = glm::translate(view, -cameraPosition);
    return view;
}
glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(fovY, aspectRatio, nearClip, farClip);
}

void Camera::rotate(const glm::vec2& rotationYX)
{
    cameraOrientation += rotationYX;
}

void Camera::moveLocal(const glm::vec3& inputVelocity)
{

    glm::mat4 inputRotationMatrix = glm::mat4(1.);
    inputRotationMatrix = glm::rotate(inputRotationMatrix, glm::radians(cameraOrientation.y), AXIS_RIGHT);
    inputRotationMatrix = glm::rotate(inputRotationMatrix, glm::radians(cameraOrientation.x), AXIS_UP);

    inputRotationMatrix = glm::inverse(inputRotationMatrix);
    glm::vec3 rotatedInputVelocity = glm::vec3(inputRotationMatrix * glm::vec4(inputVelocity, 0.0));

    glm::vec3 velocity = rotatedInputVelocity;
    cameraPosition += velocity;
}
