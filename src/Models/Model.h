#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <vector>

class Model
{
public:
    Model(std::vector<Mesh> meshes);
    void Draw(Shader &shader);

private:
    std::vector<Mesh> _meshes;
};

#endif // !MODEL_H
