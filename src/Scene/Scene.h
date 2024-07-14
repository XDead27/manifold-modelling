#ifndef INCLUDE_SCENE_SCENE_H_
#define INCLUDE_SCENE_SCENE_H_

#include "Camera.h"
#include "Light.h"
#include <Scene/Actor.h>
#include <vector>

struct Scene
{
    std::vector<Actor*> _actors;
    std::vector<Light*> _lights;
    Camera* _camera;
};

#endif  // INCLUDE_SCENE_SCENE_H_
