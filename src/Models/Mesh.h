#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <Shaders/Shader.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

enum TextureType {
    TT_DIFFUSE,
    TT_SPECULAR,
    TT_NORMAL,
    TT_HEIGHT
};

struct Texture {
    GLuint id;
    TextureType type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex>     vertices;
    std::vector<GLuint>     indices;
    std::vector<Texture>    textures;

    Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, GLenum drawMode, GLenum primitiveType);
    ~Mesh();

    void Draw(Shader &shader);
    void DrawGeometry();
private:
    GLuint _vao, _vbo, _ebo;
    GLenum _drawMode, _primitiveType;
    
    void setupMesh();
};

#endif
