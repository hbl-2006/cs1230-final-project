#include "glm/gtx/string_cast.hpp"
#include "realtime.h"
#include <iostream>
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
    if (scalarRelativeVelocity < 0) {
        // don't collide if they're moving apart.
        return;
    }
    glm::vec3 contactPoint = approximateContactPoint(A, B);
    // effectively do the ctm transformation for the COM to get its position in the world.
    glm::vec3 ACOM = A->position + (A->rot_matrix * (A->scale * A->objSpaceCOM));
    glm::vec3 BCOM = B->position + (B->rot_matrix * (B->scale * B->objSpaceCOM));
    glm::vec3 rA = contactPoint - ACOM;
    glm::vec3 rB = contactPoint - BCOM;
    // TODO: make this not a magic number lol
    float restitution = 0.8;
    if (A->mass_inv == 0) {
        B->position += mtv;
        // scalar impulse formula taken from SIGGRAPH notes 2 on rigid body constraints, page 17, but A is gone in the limit
        float numerator = (1 + restitution) * scalarRelativeVelocity;
        glm::vec3 rbCrossN = glm::cross(rB, normalizedMTV);
        float rotTermB = glm::dot(rbCrossN, B->I_world_inv * rbCrossN);
        float scalarImpulse = numerator / (B->mass_inv + rotTermB);
        glm::vec3 impulse = normalizedMTV * scalarImpulse;
        B->addImpulse(impulse);
        // Torque is distance x force, so angular impulse is distance x linear impulse by analogy
        glm::vec3 angularImpulse = glm::cross(rB, impulse);
        B->addAngularImpulse(angularImpulse);
    } else if (B->mass_inv == 0) {
        // the mtv points away from A, so we need to flip these.
        A->position -= mtv;
        // scalar impulse formula taken from SIGGRAPH notes 2 on rigid body constraints, page 17, but B is gone in the limit
        float numerator = (1 + restitution) * scalarRelativeVelocity;
        glm::vec3 raCrossN = glm::cross(rA, normalizedMTV);
        float rotTermA = glm::dot(raCrossN, A->I_world_inv * raCrossN);
        float scalarImpulse = numerator / (A->mass_inv + rotTermA);
        // negative MTV direction for A, since mtv by convention points towards B
        glm::vec3 impulse = -normalizedMTV * scalarImpulse;
        A->addImpulse(impulse);
        // Torque is distance x force, so angular impulse is distance x linear impulse by analogy
        glm::vec3 angularImpulse = glm::cross(rA, impulse);
        A->addAngularImpulse(angularImpulse);
    } else {
        A->position -= mtv / 2.0f;
        B->position += mtv / 2.0f;
        // scalar impulse formula taken from SIGGRAPH notes 2 on rigid body constraints, page 17
        glm::vec3 raCrossN = glm::cross(rA, normalizedMTV);
        glm::vec3 rbCrossN = glm::cross(rB, normalizedMTV);
        float rotTermA = glm::dot(raCrossN, A->I_world_inv * raCrossN);
        float rotTermB = glm::dot(rbCrossN, B->I_world_inv * rbCrossN);
        float numerator = (1 + restitution) * scalarRelativeVelocity;
        float denominator = A->mass_inv + B->mass_inv + rotTermA + rotTermB;
        float scalarImpulse = numerator / denominator;
        glm::vec3 impulse = normalizedMTV * scalarImpulse;
        A->addImpulse(-impulse);
        B->addImpulse(impulse);

        // Torque is distance x force, so angular impulse is distance x linear impulse by analogy
        glm::vec3 angularImpulseA = glm::cross(rA, -impulse);
        A->addAngularImpulse(angularImpulseA);

        glm::vec3 angularImpulseB = glm::cross(rB, impulse);
        B->addAngularImpulse(angularImpulseB);
    }
}

glm::vec3 Realtime::calculateMTV(RigidBody *A, RigidBody *B)
{
    // First, we need to set up our separating axes.
    BoundingBox ABox = A->box;
    BoundingBox BBox = B->box;
    glm::vec3 axes[15] = {ABox.world_space_axes[0],
                          ABox.world_space_axes[1],
                          ABox.world_space_axes[2],
                          BBox.world_space_axes[0],
                          BBox.world_space_axes[1],
                          BBox.world_space_axes[2],
                          glm::cross(ABox.world_space_axes[0], BBox.world_space_axes[0]),
                          glm::cross(ABox.world_space_axes[0], BBox.world_space_axes[1]),
                          glm::cross(ABox.world_space_axes[0], BBox.world_space_axes[2]),
                          glm::cross(ABox.world_space_axes[1], BBox.world_space_axes[0]),
                          glm::cross(ABox.world_space_axes[1], BBox.world_space_axes[1]),
                          glm::cross(ABox.world_space_axes[1], BBox.world_space_axes[2]),
                          glm::cross(ABox.world_space_axes[2], BBox.world_space_axes[0]),
                          glm::cross(ABox.world_space_axes[2], BBox.world_space_axes[1]),
                          glm::cross(ABox.world_space_axes[2], BBox.world_space_axes[2])};
    float minOverlap = FLT_MAX;
    glm::vec3 mtv(0.0f);
    for (int i = 0; i < 15; i++) {
        // if they don't overlap on any separating axis, they don't collide. 0 mtv.
        glm::vec3 normedAxis = glm::normalize(axes[i]);
        float overlap = ABox.overlapOnAxis(BBox, normedAxis);
        if (overlap == 0.0f) {
            return glm::vec3(0);
        }
        if (abs(overlap) < minOverlap) {
            minOverlap = abs(overlap);
            mtv = normedAxis * overlap;
        }
    }
    // check to make sure mtv points from A->B for our convention.
    glm::vec3 centerA = 0.5f * (ABox.min_world + ABox.max_world);
    glm::vec3 centerB = 0.5f * (BBox.min_world + BBox.max_world);
    if (glm::dot(centerB - centerA, mtv) < 0)
        mtv = -mtv;
    return mtv;
}

bool pointInsideOBB(const glm::vec3 &P, const glm::vec3 axes[3], const BoundingBox &box)
{
    // project P onto each box axis
    for (int i = 0; i < 3; i++) {
        float minProj = glm::dot(box.world_space_corners[0], axes[i]);
        float maxProj = minProj;
        for (int j = 1; j < 8; j++) {
            float proj = glm::dot(box.world_space_corners[j], axes[i]);
            if (proj < minProj)
                minProj = proj;
            if (proj > maxProj)
                maxProj = proj;
        }

        float pProj = glm::dot(P, axes[i]);
        if (pProj < minProj || pProj > maxProj)
            return false;
    }
    return true;
}

glm::vec3 Realtime::approximateContactPoint(RigidBody *A, RigidBody *B)
{
    std::vector<glm::vec3> contactCandidates;

    // 1. Add vertices of A inside B
    for (int i = 0; i < 8; i++) {
        glm::vec3 v = A->box.world_space_corners[i];
        if (pointInsideOBB(v, B->box.world_space_axes, B->box)) // see below
            contactCandidates.push_back(v);
    }

    // 2. Add vertices of B inside A
    for (int i = 0; i < 8; i++) {
        glm::vec3 v = B->box.world_space_corners[i];
        if (pointInsideOBB(v, A->box.world_space_axes, A->box))
            contactCandidates.push_back(v);
    }

    // 3. Approximate edges by connecting corner pairs for each box
    int edgePairs[12][2] = {{0, 1},
                            {0, 2},
                            {0, 4},
                            {1, 3},
                            {1, 5},
                            {2, 3},
                            {2, 6},
                            {3, 7},
                            {4, 5},
                            {4, 6},
                            {5, 7},
                            {6, 7}};

    auto sampleEdges = [&](BoundingBox &box, BoundingBox &other) {
        for (int e = 0; e < 12; e++) {
            glm::vec3 p0 = box.world_space_corners[edgePairs[e][0]];
            glm::vec3 p1 = box.world_space_corners[edgePairs[e][1]];
            for (float t = 0; t <= 1.0f; t += 0.25f) {
                glm::vec3 pt = p0 + t * (p1 - p0);
                if (pointInsideOBB(pt, other.world_space_axes, other))
                    contactCandidates.push_back(pt);
            }
        }
    };

    sampleEdges(A->box, B->box);
    sampleEdges(B->box, A->box);

    // 4. Average all candidates
    if (contactCandidates.empty()) {
        return 0.5f * (A->position + B->position);
    }

    glm::vec3 sum(0.0f);
    for (auto &p : contactCandidates)
        sum += p;
    return sum / static_cast<float>(contactCandidates.size());
}
