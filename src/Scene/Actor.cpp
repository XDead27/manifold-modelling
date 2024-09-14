#include "Actor.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void Actor::updateShader(Shader& shader) const
{
    SceneObject::updateShader(shader);

    shader.setMat4("model", _getModelMatrix());
    shader.setMat4("hyperRot", getComposedGV().toMatrix());

    _activeModel->Draw(shader);
}

glm::mat4 Actor::_getModelMatrix() const
{
    glm::mat4 outMat = glm::mat4(1.0f);
    outMat = glm::translate(outMat, getTotalTranslation());
    outMat = glm::rotate(outMat, this->getRotation().x, {1.0f, 0.0f, 0.0f});
    outMat = glm::rotate(outMat, this->getRotation().y, {0.0f, 1.0f, 0.0f});
    outMat = glm::rotate(outMat, this->getRotation().z, {0.0f, 0.0f, 1.0f});
    outMat = glm::scale(outMat, getScale());

    return outMat;
}

glm::vec3 Actor::getTotalTranslation() const
{
    return getTranslation() + this->_translationOffset;
}

void Actor::addTranslationOffset(glm::vec3 offset)
{
    this->_translationOffset += offset;
}

void Actor::setTranslationOffset(glm::vec3 offset)
{
    this->_translationOffset = offset;
}
