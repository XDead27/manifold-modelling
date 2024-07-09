#include "Renderer.h"
#include <GLFW/glfw3.h>

void render() {
    // Render implementation
}

void renderModel() {
}

void renderPicking(Shader* pickingShader, GLuint pickingFBO, std::map<GLint, glm::vec3> modelColors) {
    glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& [id, color] : modelColors) {
        // Set unique color for each model
	pickingShader->setVec3("objectColor", color);
        
        // Render model with id
        renderModel();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


