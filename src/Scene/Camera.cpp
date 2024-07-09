#include "Camera.h"

#include <Scene/SceneObject.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(
    glm::vec3 translation,
    glm::vec3 rotation,
    glm::vec3 scale,
    glm::vec3 worldUp,
    float mvmtSpeed,
    float mouseSensitivity,
    float zoom
) : SceneObject(translation, rotation, scale)
{
    this->worldUp = worldUp;
    this->movementSpeed = mvmtSpeed;
    this->mouseSensitivity = mouseSensitivity;
    this->zoom = zoom;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(getTranslation(), getTranslation() + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == CM_FORWARD)
        addTranslation(front * velocity);
    if (direction == CM_BACKWARD)
        addTranslation(-front * velocity);
    if (direction == CM_LEFT)
        addTranslation(-right * velocity);
    if (direction == CM_RIGHT)
        addTranslation(right * velocity);
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    glm::vec3 rot = addRotation({0.0f, xoffset, yoffset});

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (rot.z > 89.0f)
            rot.z = 89.0f;
        if (rot.z < -89.0f)
            rot.z = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 rot = getRotation();
    glm::vec3 local_front;
    local_front.x = cos(glm::radians(rot.y)) * cos(glm::radians(rot.z));
    local_front.y = sin(glm::radians(rot.z));
    local_front.z = sin(glm::radians(rot.y)) * cos(glm::radians(rot.z));
    front = glm::normalize(local_front);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up    = glm::normalize(glm::cross(right, front));
}
