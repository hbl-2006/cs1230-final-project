#include "realtime.h"
void Realtime::resolveOneCollision(RigidBody *A, RigidBody *B)
{
    glm::vec3 mtv = calculateMTV(A, B);
    // skip tiny mtv, we don't want NaNs.
    if (glm::length(mtv) < 1e-6f)
        return;

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
        float denominator = (B->mass_inv + rotTermB);
        if (denominator < 1e-6f)
            denominator = 1e-6f;
        float scalarImpulse = numerator / denominator;
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
        // make sure denominator isn't zero
        float denominator = (A->mass_inv + rotTermA);
        if (denominator < 1e-6f)
            denominator = 1e-6f;
        float scalarImpulse = numerator / denominator;
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
        // make sure denominator isn't zero
        if (denominator < 1e-6f)
            denominator = 1e-6f;
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

bool pointInsideOBB(const glm::vec3 &P, const BoundingBox &box, glm::vec3 center)
{
    glm::vec3 d = P - center;

    for (int i = 0; i < 3; i++) {
        float dist = glm::dot(d, box.world_space_axes[i]);
        if (fabs(dist) > box.world_space_halfWidth[i])
            return false;
    }
    return true;
}

glm::vec3 Realtime::approximateContactPoint(RigidBody *A, RigidBody *B)
{
    std::vector<glm::vec3> contactCandidates;

    // first just check if any vertex is inside the other box: easy
    for (int i = 0; i < 8; i++) {
        glm::vec3 v = A->box.world_space_corners[i];
        if (pointInsideOBB(v, B->box, B->position))
            contactCandidates.push_back(v);
    }

    for (int i = 0; i < 8; i++) {
        glm::vec3 v = B->box.world_space_corners[i];
        if (pointInsideOBB(v, A->box, A->position))
            contactCandidates.push_back(v);
    }

    // for our edges, we have each pair of corners
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

    // function that samples points along the edges to look for potential contacts
    auto sampleEdges = [&](const RigidBody *body, const RigidBody *other) {
        for (int e = 0; e < 12; e++) {
            glm::vec3 p0 = body->box.world_space_corners[edgePairs[e][0]];
            glm::vec3 p1 = body->box.world_space_corners[edgePairs[e][1]];
            for (float t : {0.0f, 0.5f, 1.0f}) {
                glm::vec3 pt = p0 + t * (p1 - p0);
                if (pointInsideOBB(pt, other->box, other->position))
                    contactCandidates.push_back(pt);
            }
        }
    };

    // only check edges if we havent found any candidates yet
    if (contactCandidates.empty()) {
        sampleEdges(A, B);
        sampleEdges(B, A);
    }

    if (contactCandidates.empty()) {
        // if we're STILL empty, just use midpoint between centers
        return 0.5f * (A->position + B->position);
    }

    // average every potential contact point to get an overall estimate for contact.
    glm::vec3 sum(0.0f);
    for (auto &p : contactCandidates)
        sum += p;
    return sum / (float) contactCandidates.size();
}
