#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <glm/glm.hpp>
#include <Shaders/Shader.h>
#include <string>

class SceneObject
{
private:
    glm::vec3 _translation;
    glm::vec3 _rotation;
    glm::vec3 _scale;

    std::string _name;

public:
    SceneObject(
        std::string name,
        glm::vec3 tra = {0.0f, 0.0f, 0.0f}, 
        glm::vec3 rot = {0.0f, 0.0f, 0.0f}, 
        glm::vec3 sca = {1.0f, 1.0f, 1.0f}
    ) :
    _translation(tra),
    _rotation(rot),
    _scale(sca),
    _name(name)
    {}

    glm::vec3 getTranslation() const;
    glm::vec3 getRotation() const;
    glm::vec3 getScale() const;
    std::string getName() const;

    void setTranslation(glm::vec3);
    void setRotation(glm::vec3);
    void setScale(glm::vec3);

    glm::vec3 addTranslation(glm::vec3);
    glm::vec3 addRotation(glm::vec3);
    glm::vec3 scaleBy(glm::vec3);

    virtual void updateShader(Shader& shader) const = 0;
};

#endif
