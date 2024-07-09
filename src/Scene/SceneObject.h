#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <glm/glm.hpp>

class SceneObject
{
private:
    glm::vec3 _translation;
    glm::vec3 _rotation;
    glm::vec3 _scale;

public:
    SceneObject() : SceneObject({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}) {}
    SceneObject(glm::vec3 tra, glm::vec3 rot, glm::vec3 sca);

    glm::vec3 getTranslation();
    glm::vec3 getRotation();
    glm::vec3 getScale();

    void setTranslation(glm::vec3);
    void setRotation(glm::vec3);
    void setScale(glm::vec3);

    glm::vec3 addTranslation(glm::vec3);
    glm::vec3 addRotation(glm::vec3);
    glm::vec3 scaleBy(glm::vec3);
};

#endif
