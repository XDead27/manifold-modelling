#include "GyroVector3D.h"

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
