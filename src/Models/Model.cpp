#include "Model.h"

void Model::Draw(Shader& shader)
{
    for (Mesh* m : _meshes) {
        m->Draw(shader);
    }
}
