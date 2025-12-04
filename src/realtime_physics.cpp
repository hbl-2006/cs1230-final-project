#include "realtime.h"

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
        glm::vec3 min = init;
        glm::vec3 max = init;
        for (int i = 1; i < 8; i++) {
            auto worldVec = glm::vec3(shape.ctm * glm::vec4(box.obj_space_corners[i], 1));
            // Take the element-wise min and max of our current min and max with this vector, eventually builds
            // world space min+max
            min = glm::min(min, worldVec);
            max = glm::max(max, worldVec);
        }
        box.min_world = min;
        box.max_world = max;
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

void Realtime::resolveOneCollision(RigidBody *A, RigidBody *B)
{
    glm::vec3 mtv = calculateMTV(A, B);
    // skip zero mtv, we don't want NaNs.
    if (mtv == glm::vec3(0.0f)) {
        return;
    }
    // We have cases here: both are static, one is static, or neither is static.
    if (A->mass_inv == 0 && B->mass_inv == 0) {
        // Two static objects colliding doesn't actually do anything, since they can't move.
        return;
    }
    // We now have the actual cases where we resolve collisions!
    glm::vec3 normalizedMTV = glm::normalize(mtv);
    float scalarRelativeVelocity = glm::dot(A->velocity - B->velocity, normalizedMTV);
    // TODO: make this not a magic number lol
    float restitution = 0.8;
    if (A->mass_inv == 0) {
        B->position += mtv;
        glm::vec3 impulse = normalizedMTV * (scalarRelativeVelocity * (1 + restitution) * B->mass);
        B->addImpulse(impulse);
    } else if (B->mass_inv == 0) {
        A->position += mtv;
        glm::vec3 impulse = normalizedMTV * (scalarRelativeVelocity * (1 + restitution) * A->mass);
        A->addImpulse(impulse);
    } else {
        A->position -= mtv / 2.0f;
        B->position += mtv / 2.0f;
        float scalarImpulse = ((A->mass * B->mass) * scalarRelativeVelocity * (1 + restitution))
                              / (A->mass + B->mass);
        glm::vec3 impulse = normalizedMTV * scalarImpulse;
        A->addImpulse(-impulse);
        B->addImpulse(impulse);
    }
}

glm::vec3 Realtime::calculateMTV(RigidBody *A, RigidBody *B)
{
    glm::vec3 mtv(0.0f);

    // Pick the axis with the least overlap, and that overlap is our MTV.
    float overlapX = std::max(0.0f,
                              std::min(A->box.max_world.x, B->box.max_world.x)
                                  - std::max(A->box.min_world.x, B->box.min_world.x));
    float overlapY = std::max(0.0f,
                              std::min(A->box.max_world.y, B->box.max_world.y)
                                  - std::max(A->box.min_world.y, B->box.min_world.y));
    float overlapZ = std::max(0.0f,
                              std::min(A->box.max_world.z, B->box.max_world.z)
                                  - std::max(A->box.min_world.z, B->box.min_world.z));

    // pick the smallest overlap of these three
    float minOverlapMagnitude = std::min(overlapX, std::min(overlapY, overlapZ));
    if (minOverlapMagnitude <= 0.0f) {
        return glm::vec3(0.0f);
    }

    if (minOverlapMagnitude == overlapX) {
        mtv = glm::vec3(overlapX, 0, 0);
    } else if (minOverlapMagnitude == overlapY) {
        mtv = glm::vec3(0, overlapY, 0);
    } else {
        mtv = glm::vec3(0, 0, overlapZ);
    }

    // Check direction of MTV: we'll enforce that it points from A to B
    glm::vec3 centerA = 0.5f * (A->box.min_world + A->box.max_world);
    glm::vec3 centerB = 0.5f * (B->box.min_world + B->box.max_world);

    // if the mtv is negative with respect to a line from A->B, we should flip it so it points from A->B.
    if (glm::dot(centerB - centerA, mtv) < 0.0f)
        mtv = -mtv;

    return mtv;
}
