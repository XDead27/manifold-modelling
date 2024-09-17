#ifndef INCLUDE_MATH_GYROVECTOR3D_H_
#define INCLUDE_MATH_GYROVECTOR3D_H_

#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class GyroVector3D
{
public:
    // Members
    glm::vec3 vec;
    glm::quat gyr;

    GyroVector3D(glm::vec3 vec, glm::quat gyr) : vec(vec), gyr(gyr) {}
    GyroVector3D(float x, float y, float z) : GyroVector3D(glm::vec3(x, y, z), glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {}
    GyroVector3D() : GyroVector3D(glm::vec3(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {}

    GyroVector3D operator+(GyroVector3D b);
    GyroVector3D operator+(glm::vec3 b);
    GyroVector3D operator+=(glm::vec3 b);
    GyroVector3D operator-();
    GyroVector3D operator-(GyroVector3D b);
    GyroVector3D operator-(glm::vec3 b);
    GyroVector3D operator-=(glm::vec3 b);
    glm::vec3 operator*(glm::vec3 f);
    

    glm::mat4 toMatrix();

private:
    
};

#endif  // INCLUDE_MATH_GYROVECTOR3D_H_
