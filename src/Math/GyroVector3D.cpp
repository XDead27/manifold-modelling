#include "GyroVector3D.h"
#include "glm/ext/matrix_transform.hpp"
#include <Math/Manifold.h>

GyroVector3D GyroVector3D::operator+(GyroVector3D b)
{
    glm::vec3 x = glm::inverse(gyr) * b.vec;
    glm::vec3 cVec = Manifold::K * glm::cross(vec, x);
    float d = 1.0f - Manifold::K * glm::dot(vec, x);
    glm::vec3 t = vec + x;
    glm::vec3 sum = (t * d + glm::cross(cVec, t)) / (d * d + glm::length2(cVec));
    glm::quat nGyr = glm::quat(-cVec.x, -cVec.y, -cVec.z, d);
    return GyroVector3D(sum, b.gyr * gyr * nGyr);
}

GyroVector3D GyroVector3D::operator-()
{
    return GyroVector3D(-(gyr * vec), glm::inverse(gyr));
}

GyroVector3D GyroVector3D::operator-(GyroVector3D b)
{
    return *this + -b;
}

glm::vec3 GyroVector3D::operator*(glm::vec3 f)
{
    // Compute mobius addition
    glm::vec3 a = vec;
    glm::vec3 b = f;

    glm::vec3 c = Manifold::K * glm::cross(a, b);
    float d = 1.0f - Manifold::K * glm::dot(a, b);
    glm::vec3 t = a + b;

    glm::vec3 ma = (t * d + glm::cross(c, t)) / (d * d + glm::length2(c));

    return gyr * ma;
}

glm::mat4 GyroVector3D::toMatrix()
{
    return glm::translate(glm::mat4(1.0f), vec) *
        glm::toMat4(gyr);
}
