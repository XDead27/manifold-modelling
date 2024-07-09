#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>

#include <Shaders/Shader.h>

void render();

void renderModel();

void renderPicking(Shader* pickingShader, GLuint pickingFBO, std::map<GLint, glm::vec3> modelColors);

#endif

