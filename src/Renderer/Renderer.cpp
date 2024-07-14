#include "Renderer.h"
#include "Camera.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <Utils/Misc.h>

Renderer::Renderer
(
    Shader *defaultShader,
    Shader *pickingShader,
    unsigned int screenWidth,
    unsigned int screenHeight
) :
    _defaultShader(defaultShader),
    _pickingShader(pickingShader),
    _screenWidth(screenWidth),
    _screenHeight(screenHeight)
{
    _setupPickingFramebuffer();
}

void Renderer::renderScene(Scene& scene)
{
    // first pass: picking framebuffer
    // -------------------------------
    GLuint id = 0;
    std::map<GLuint, Actor*> actorIds;
    for (Actor* a : scene._actors) {
        actorIds[id++] = a;
    }
    _renderPicking(actorIds, scene._camera);
    _currentActorIds = actorIds;

    // second pass: default framebuffer
    // --------------------------------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate shader
    _defaultShader->use();

    // set light attributes
    for (Light* l : scene._lights) {
        l->updateShader(*_defaultShader);
    }
    scene._camera->updateShader(*_defaultShader);
    _defaultShader->setFloat("material.shininess", 32);

    for (Actor* a : scene._actors) {
        a->updateShader(*_defaultShader);
    }
}

Actor* Renderer::getActorAtPosition(glm::vec2 pos)
{
    // Read pixel color from picking buffer
    glBindFramebuffer(GL_FRAMEBUFFER, _pickingFBO);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    float data[3];
    glReadPixels(pos.x, _screenHeight - pos.y, 1, 1, GL_RGB, GL_FLOAT, data);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Convert color to model ID
    GLint id = getIDFromColor(data);

    if (id >= 0)
        return _currentActorIds[id];
    else 
        return NULL;
}

void Renderer::_renderPicking(std::map<GLuint, Actor*> actors, Camera* camera)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _pickingFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    _pickingShader->use();
    camera->updateShader(*_pickingShader);

    for (const auto& [id, actor] : actors) {
        // Set unique color for each model
        glm::vec3 modelColor = getColorFromID(id);
	_pickingShader->setVec3("objectColor", modelColor);
        
        // Render model with id
        actor->updateShader(*_pickingShader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::_setupPickingFramebuffer()
{
    // setup picking buffers
    // ---------------------
    GLuint pickingColorBuffer, pickingDepthBuffer;
    glGenFramebuffers(1, &this->_pickingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _pickingFBO);

    // create color attachment
    // -----------------------
    glGenTextures(1, &pickingColorBuffer);
    glBindTexture(GL_TEXTURE_2D, pickingColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _screenWidth, _screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickingColorBuffer, 0);

    // create depth attachment
    // -----------------------
    glGenRenderbuffers(1, &pickingDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, pickingDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pickingDepthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Error: Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
