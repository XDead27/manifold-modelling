#include "Mesh.h"
#include <numeric>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures)
{
    std::vector<GLuint> genIndices(vertices.size());
    std::iota(genIndices.begin(), genIndices.end(), 0);
    Mesh(vertices, genIndices, textures, GL_STATIC_DRAW, GL_TRIANGLES);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
    Mesh(vertices, indices, textures, GL_STATIC_DRAW, GL_TRIANGLES);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, GLenum drawMode, GLenum primitiveType)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->_drawMode = drawMode;
    this->_primitiveType = primitiveType;

    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], _drawMode);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), 
                 &indices[0], _drawMode);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader) 
{
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number, name;
        TextureType ttype = textures[i].type;
        if(ttype == TT_DIFFUSE) {
            number = std::to_string(diffuseNr++);
            name = "texture_diffuse";
        }
        else if(ttype == TT_SPECULAR) {
            number = std::to_string(specularNr++);
            name = "texture_specular";
        }

        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    DrawGeometry();
}

void Mesh::DrawGeometry()
{
    // draw mesh
    glBindVertexArray(_vao);
    glDrawElements(_primitiveType, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}
