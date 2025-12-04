#ifndef RIGID_BODY_H
#define RIGID_BODY_H
#include "glm/gtx/string_cast.hpp"
#include "physics/bounding_box.h"
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <ostream>
struct RigidBody
{
    BoundingBox box;

    float mass = 1.0f;
    float mass_inv = 1.0f;
    glm::mat3 I_obj;
    glm::mat3 I_obj_inv;

    glm::vec3 objSpaceCOM = glm::vec3(0.0f);
    glm::vec3 scale;

    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 linear_momentum;
    glm::vec3 angular_momentum;

    glm::mat3 I_world_inv;
    glm::mat3 rot_matrix;
    glm::vec3 velocity;
    glm::vec3 omega;

    glm::vec3 force;
    glm::vec3 torque;

    void addForce(glm::vec3 f) { force += f; }

    void addImpulse(glm::vec3 J) { linear_momentum += J; }

    void addTorque(glm::vec3 t) { torque += t; }

    void addAngularImpulse(glm::vec3 K) { angular_momentum += K; }

    bool physicsStep(float dt)
    {
        if (mass_inv == 0.0f)
            return false;
        linear_momentum += dt * force;
        angular_momentum += dt * torque;

        // I love how easy linear stuff is in comparison to rotation stuff: just calculate velocity and integrate position based on time
        velocity = linear_momentum * mass_inv;
        position += velocity * dt;

        // Equivalent to getting velocity, we need to get omega: we get the rotation matrix as we are right now to figure out our inertia tensor in world space,
        // then update omega based on physics formulas
        rot_matrix = glm::mat3_cast(orientation);
        I_world_inv = rot_matrix * I_obj_inv * glm::transpose(rot_matrix);
        omega = I_world_inv * angular_momentum;

        // Rotational equivalent to updating position: as explained in the paper - treat the multiplication of omega and quaternion as [0, omega]q
        glm::quat omega_quat(0, omega.x, omega.y, omega.z);
        orientation += 0.5f * (omega_quat * orientation) * dt;
        orientation = glm::normalize(orientation);

        // Take the rotation matrix of our new orientation after updating for actual rendering purposes
        rot_matrix = glm::mat3_cast(orientation);

        force = glm::vec3(0);
        torque = glm::vec3(0);
        return true;
    }
};

#endif // RIGID_BODY_H
