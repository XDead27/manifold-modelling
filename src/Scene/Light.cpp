#include "Light.h"

void Light::updateShader(Shader& shader) const
{
    shader.setVec3(("light_" + this->getName() + ".ambient").c_str(), this->_ambient);
    shader.setVec3(("light_" + this->getName() + ".diffuse").c_str(), this->_diffuse);
    shader.setVec3(("light_" + this->getName() + ".specular").c_str(), this->_specular);
    shader.setVec3(("light_" + this->getName() + ".position").c_str(), this->getTranslation());
}
