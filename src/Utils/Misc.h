#ifndef INCLUDE_UTILS_MISC_H_
#define INCLUDE_UTILS_MISC_H_

#include <glm/glm.hpp>

glm::vec3 getColorFromID(int id);
int getIDFromColor(glm::vec3 color);
int getIDFromColor(float color[3]);

#endif  // INCLUDE_UTILS_MISC_H_
