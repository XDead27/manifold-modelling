#include "Misc.h"

glm::vec3 getColorFromID(int id)
{
    id++;
    int r = (id & 0x000000FF) >>  0;
    int g = (id & 0x0000FF00) >>  8;
    int b = (id & 0x00FF0000) >> 16;

    return {r / 255.0f, g / 255.0f, b / 255.0f};
}

int getIDFromColor(glm::vec3 color)
{
    float pickedID = 
        color[0] * 255.0f + 
        color[1] * 255.0f * 256.0f +
        color[2] * 255.0f * 256.0f * 256.0f;

    int res = static_cast<int>(pickedID);
    return --res;
}

int getIDFromColor(float color[3])
{
    float pickedID = 
        color[0] * 255.0f + 
        color[1] * 255.0f * 256.0f +
        color[2] * 255.0f * 256.0f * 256.0f;

    int res = static_cast<int>(pickedID);
    return --res;
}
