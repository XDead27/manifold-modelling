#ifndef INCLUDE_SCENE_ACTOR_H_
#define INCLUDE_SCENE_ACTOR_H_

#include "Model.h"
#include <Scene/SceneObject.h>
#include <Shaders/Shader.h>

class Actor : public SceneObject 
{
public:
    Actor(std::string name, Model* activeModel) : SceneObject(name), _activeModel(activeModel), _translationOffset(0.0f) {}

    void updateShader(Shader& shader) const override;

    glm::vec3 getTotalTranslation() const;
    void addTranslationOffset(glm::vec3 offset);
    void setTranslationOffset(glm::vec3 offset);

private:
    Model* _activeModel;
    glm::vec3 _translationOffset;

    glm::mat4 _getModelMatrix() const;
};

#endif  // INCLUDE_SCENE_ACTOR_H_
