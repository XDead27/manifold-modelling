#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <assimp/scene.h>
#include <string>
#include <vector>

class Model
{
public:
    Model(const char* path) { _loadModel(path); }
    Model(std::vector<Mesh*>& meshes) : _meshes(meshes) {}
    ~Model();
    void Draw(Shader &shader);

private:
    std::vector<Mesh*> _meshes;
    std::string _directory;
    std::vector<Texture> _textures_loaded;
    bool _gammaCorrection;

    void _loadModel(std::string path);
    void _processNode(aiNode* node, const aiScene *scene);
    Mesh* _processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> _loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType typeName);
};

#endif // !MODEL_H
