#include "util_funcs.h"
void insertVec3(std::vector<float> &data, glm::vec3 v)
{
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
