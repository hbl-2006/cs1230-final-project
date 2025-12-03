#ifndef UTIL_FUNCS_H
#define UTIL_FUNCS_H
#include <vector>
#include <glm/glm.hpp>
// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void insertVec3(std::vector<float> &data, glm::vec3 v);
#endif // UTIL_FUNCS_H
