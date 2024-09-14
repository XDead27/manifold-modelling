#include "SceneObject.h"
#include <Math/GyroVector3D.h>
#include "glm/fwd.hpp"
#include <string>

GyroVector3D SceneObject::worldGV = GyroVector3D();

glm::vec3 SceneObject::getTranslation() const
{
    return this->_translation;
}

glm::vec3 SceneObject::getRotation() const
{
    return this->_rotation;
}

glm::vec3 SceneObject::getScale() const
{
    return this->_scale;
}

std::string SceneObject::getName() const
{
    return this->_name;
}

GyroVector3D SceneObject::getComposedGV() const
{
    return this->_composedGV;
}

void SceneObject::setTranslation(glm::vec3 newTra)
{
    this->_translation = newTra;
}

void SceneObject::setRotation(glm::vec3 newRot)
{
    this->_rotation = newRot;
}

void SceneObject::setScale(glm::vec3 newSca)
{
    this->_scale = newSca;
}

glm::vec3 SceneObject::addTranslation(glm::vec3 addTra)
{
    this->_translation += addTra;
    return this->_translation;
}

glm::vec3 SceneObject::addRotation(glm::vec3 addRot)
{
    this->_rotation += addRot;
    return this->_rotation;
}

glm::vec3 SceneObject::scaleBy(glm::vec3 scBy)
{
    this->_scale *= scBy;
    return this->_scale;
}
