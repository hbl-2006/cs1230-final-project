#include "glm/gtx/string_cast.hpp"
#include "realtime.h"
#include <iostream>

void Realtime::stepPhysics(float deltaTime)
{
    for (auto &shape : metadata.shapes) {
        RigidBody &body = shape.body;
        // step through physics, which will update our position and rotation matrix. remake ctm and inverse ctm!
        if (gravity) {
            body.addForce(glm::vec3(0, -3, 0));
        }
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

inline void Realtime::updateBoundingBoxes()
{
    for (auto &shape : metadata.shapes) {
        if (shape.ctm == shape.last_ctm)
            continue;
        shape.last_ctm = shape.ctm;
        RigidBody &body = shape.body;
        BoundingBox &box = body.box;
        //extract features from ctm to avoid 4x4 matmuls, optimization
        glm::mat3 linear = glm::mat3(shape.ctm);
        glm::vec3 translation = glm::vec3(shape.ctm[3]);

        // set up rotated box by setting its world-space basis axes.
        box.world_space_axes[0] = glm::normalize(linear[0]);
        box.world_space_axes[1] = glm::normalize(linear[1]);
        box.world_space_axes[2] = glm::normalize(linear[2]);
        // We don't care about rotation and translation for half width, but we do for scale.
        box.world_space_halfWidth = glm::mat3(shape.scale) * box.obj_space_halfWidth;

        // Get world space corners by multiplying linear transformation then adding translation.
        for (int i = 0; i < 8; ++i) {
            glm::vec3 corner = box.obj_space_corners[i];
            box.world_space_corners[i] = linear * corner + translation;
        }

        // Figure out extents of our OBB, then wrap it in a big AABB for sweep and prune.
        glm::vec3 min = box.world_space_corners[0];
        glm::vec3 max = min;
        for (int i = 1; i < 8; ++i) {
            min = glm::min(min, box.world_space_corners[i]);
            max = glm::max(max, box.world_space_corners[i]);
        }
        box.min_world = min;
        box.max_world = max;
    }
}

void Realtime::sweepAndPrune()
{
    // Sort along one axis, this gives us a chance to exit checks early.
    std::sort(sortedBodies.begin(), sortedBodies.end(), [](const RigidBody *a, const RigidBody *b) {
        return a->box.min_world.x < b->box.min_world.x;
    });

    for (int i = 0; i < sortedBodies.size(); i++) {
        RigidBody *A = sortedBodies[i];
        // Inner loop structured such that we check each pair only once.
        for (int j = i + 1; j < sortedBodies.size(); j++) {
            RigidBody *B = sortedBodies[j];
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
