#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
// Bounding box structure for collisions: all of our default primitives have the same bounding box in object space.
struct Interval
{
    float min;
    float max;

    float mtv(const Interval& other)
    {
        float right = other.max - min;
        float left = max - other.min;
        if (right <= 0 || left <= 0) {
            return 0.0f;
        }
        if (right < left) {
            return right;
        } else {
            return -left;
        }
    }
};

struct BoundingBox
{
    glm::vec3 min_obj = glm::vec3(-0.5);
    glm::vec3 max_obj = glm::vec3(0.5);
    glm::vec3 obj_space_corners[8] = {{min_obj.x, min_obj.y, min_obj.z},
                                      {min_obj.x, min_obj.y, max_obj.z},
                                      {min_obj.x, max_obj.y, min_obj.z},
                                      {min_obj.x, max_obj.y, max_obj.z},
                                      {max_obj.x, min_obj.y, min_obj.z},
                                      {max_obj.x, min_obj.y, max_obj.z},
                                      {max_obj.x, max_obj.y, min_obj.z},
                                      {max_obj.x, max_obj.y, max_obj.z}};
    glm::vec3 min_world;
    glm::vec3 max_world;
    glm::vec3 world_space_corners[8];
    glm::vec3 world_space_axes[3];

    // Taken from Game Physics Cookbook - returns the projection of our box along an axis.
    Interval getInterval(glm::vec3 axis) const
    {
        Interval result;
        float init = glm::dot(axis, world_space_corners[0]);
        result.min = init;
        result.max = init;

        for (int i = 1; i < 8; i++) {
            float projection = glm::dot(axis, world_space_corners[i]);
            result.min = (projection < result.min) ? projection : result.min;
            result.max = (projection > result.max) ? projection : result.max;
        }
        return result;
    }

    float overlapOnAxis(const BoundingBox& other, glm::vec3 axis)
    {
        Interval ourInterval = getInterval(axis);
        Interval otherInterval = other.getInterval(axis);
        return ourInterval.mtv(otherInterval);
    }
};
#endif // BOUNDING_BOX_H
