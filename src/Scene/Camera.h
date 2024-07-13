#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <Scene/SceneObject.h>
#include <string>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
    CM_FORWARD,
    CM_BACKWARD,
    CM_LEFT,
    CM_RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera : public SceneObject
{
public:
    // camera Attributes
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    // constructor with vectors
    Camera(std::string name) : 
        SceneObject(name), 
        worldUp({0.0f, 1.0f, 0.0f}), 
        movementSpeed(SPEED),
        mouseSensitivity(SENSITIVITY),
        zoom(ZOOM)
    {}

    Camera(
        std::string name,
        glm::vec3 translation,
        glm::vec3 rotation,
        glm::vec3 scale,
        glm::vec3 worldUp,
        float mvmtSpeed,
        float mouseSensitivity,
        float zoom
    );

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() const;

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(CameraMovement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset);

    void updateShader(Shader& shader) const override;

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
#endif
