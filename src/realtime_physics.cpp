#include "glm/gtx/string_cast.hpp"
#include "realtime.h"
#include <iostream>

void Realtime::stepPhysics(float deltaTime)
{
    for (auto &shape : metadata.shapes) {
        RigidBody &body = shape.body;
        // step through physics, which will update our position and rotation matrix. remake ctm and inverse ctm!
        body.addForce(glm::vec3(0, -1, 0));
        bool stepped = body.physicsStep(deltaTime);
        if (stepped) {
            shape.ctm = glm::translate(glm::mat4(1.0f), body.position) * glm::mat4(body.rot_matrix)
                        * shape.scale;
            shape.inverse_ctm = glm::inverse(shape.ctm);
        }
    }
}

void Realtime::resolveCollisions()
{
    // First, update our bounding boxes for every shape.
    updateBoundingBoxes();
    // Next, run collision checks (optimized with sweep and prune)
    sweepAndPrune();
}

void Realtime::updateBoundingBoxes()
{
    for (auto &shape : metadata.shapes) {
        RigidBody &body = shape.body;
        BoundingBox &box = body.box;
        glm::vec3 init = glm::vec3(shape.ctm * glm::vec4(box.obj_space_corners[0], 1));
        box.world_space_corners[0] = init;
        glm::vec3 min = init;
        glm::vec3 max = init;
        for (int i = 1; i < 8; i++) {
            auto worldVec = glm::vec3(shape.ctm * glm::vec4(box.obj_space_corners[i], 1));
            box.world_space_corners[i] = worldVec;
            // Take the element-wise min and max of our current min and max with this vector, eventually builds
            // world space min+max
            min = glm::min(min, worldVec);
            max = glm::max(max, worldVec);
        }
        // set up big AABB for the sweep and prune.
        box.min_world = min;
        box.max_world = max;

        // set up rotated box by setting its world-space basis axes.
        box.world_space_axes[0] = glm::normalize(box.world_space_corners[4]
                                                 - box.world_space_corners[0]);
        box.world_space_axes[1] = glm::normalize(box.world_space_corners[2]
                                                 - box.world_space_corners[0]);
        box.world_space_axes[2] = glm::normalize(box.world_space_corners[1]
                                                 - box.world_space_corners[0]);
    }
}

void Realtime::sweepAndPrune()
{
    std::vector<RigidBody *> sorted;
    sorted.reserve(metadata.shapes.size());
    for (auto &shape : metadata.shapes) {
        sorted.push_back(&(shape.body));
    }

    // Sort along one axis, this gives us a chance to exit checks early.
    std::sort(sorted.begin(), sorted.end(), [](const RigidBody *a, const RigidBody *b) {
        return a->box.min_world.x < b->box.min_world.x;
    });

    for (int i = 0; i < sorted.size(); i++) {
        RigidBody *A = sorted[i];
        // Inner loop structured such that we check each pair only once.
        for (int j = i + 1; j < sorted.size(); j++) {
            RigidBody *B = sorted[j];
            // Key optimization: if o2's min is greater than o1's max, and we're sorted,
            // no objects past this point will ever collide
            if (B->box.min_world.x > A->box.max_world.x) {
                break;
            }

            // At this point, we know that o2's min x is bigger than o1's min x (by sorting) and less than o1's max x,
            // so they overlap on X. Just need to make sure Y and Z also overlap.
            if (A->box.max_world.y < B->box.min_world.y || A->box.min_world.y > B->box.max_world.y) {
                continue;
            }

            if (A->box.max_world.z < B->box.min_world.z || A->box.min_world.z > B->box.max_world.z) {
                continue;
            }

            // At this point our two AABBs align, we can resolve the collision.
            resolveOneCollision(A, B);
        }
    }
}
