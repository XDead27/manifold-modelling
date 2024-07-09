#include "SceneObject.h"
#include "glm/fwd.hpp"

SceneObject::SceneObject(glm::vec3 tra, glm::vec3 rot, glm::vec3 sca)
{
    this->_translation = tra;
    this->_rotation = rot;
    this->_scale = sca;
}

glm::vec3 SceneObject::getTranslation()
{
    return this->_translation;
}

glm::vec3 SceneObject::getRotation()
{
    return this->_rotation;
}

glm::vec3 SceneObject::getScale()
{
    return this->_scale;
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
