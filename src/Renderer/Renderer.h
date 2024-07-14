#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shaders/Shader.h>
#include <Scene/Actor.h>
#include <Scene/Scene.h>

#include <map>

class Renderer
{
public:
    Renderer
    (
        Shader *defaultShader,
        Shader *pickingShader,
        unsigned int screenWidth = 800,
        unsigned int screenHeight = 600
    );

    void renderScene(Scene& scene);

    Actor* getActorAtPosition(glm::vec2 pos);

private:
    GLuint _pickingFBO;
    Shader *_defaultShader, *_pickingShader;
    unsigned int _screenWidth, _screenHeight;
    std::map<GLuint, Actor*> _currentActorIds;

    void _renderPicking(std::map<GLuint, Actor*> actors, Camera* camera);
    void _setupPickingFramebuffer();
};

#endif

