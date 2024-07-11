#ifndef LIGHT_H
#define LIGHT_H

#include <Scene/SceneObject.h>
#include <string>

class Light : public SceneObject {
private:
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;

public:
    Light
    (
        std::string name,
        glm::vec3 ambient = glm::vec3(0.2f),
        glm::vec3 diffuse = glm::vec3(0.5f),
        glm::vec3 specular = glm::vec3(1.0f)
    ) : 
    SceneObject(name),
    _ambient(ambient),
    _diffuse(diffuse),
    _specular(specular)
    {}

    /** ambient setter. */
    void setAmbient(glm::vec3 const& ambient) { this->_ambient = ambient; } 
    /** diffuse setter. */
    void setDiffuse(glm::vec3 const& diffuse) { this->_diffuse = diffuse; } 
    /** specular setter. */
    void setSpecular(glm::vec3 const& specular) { this->_specular = specular; } 

    void updateShader(Shader& shader) const override;
};

#endif // !LIGHT_H
