#include "Sphere.h"
void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    // Task 5: Implement the makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!
    auto v1 = topLeft;
    auto v2 = bottomRight;
    auto v3 = bottomLeft;
    insertVec3(m_vertexData, v1);
    insertVec3(m_vertexData, glm::normalize(v1));
    insertVec3(m_vertexData, v2);
    insertVec3(m_vertexData, glm::normalize(v2));
    insertVec3(m_vertexData, v3);
    insertVec3(m_vertexData, glm::normalize(v3));
    v1 = topLeft;
    v2 = topRight;
    v3 = bottomRight;
    insertVec3(m_vertexData, v1);
    insertVec3(m_vertexData, glm::normalize(v1));
    insertVec3(m_vertexData, v2);
    insertVec3(m_vertexData, glm::normalize(v2));
    insertVec3(m_vertexData, v3);
    insertVec3(m_vertexData, glm::normalize(v3));
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    // Task 6: create a single wedge of the sphere using the
    //         makeTile() function you implemented in Task 5
    // Note: think about how param 1 comes into play here!
    for (int i = 0; i < m_param1; i++) {
        float phi = i * M_PI / m_param1;
        float next_phi = phi + M_PI / m_param1;
        auto topLeft = 0.5f
                       * glm::vec3(glm::sin(phi) * glm::cos(currentTheta),
                                   -glm::cos(phi),
                                   -glm::sin(phi) * glm::sin(currentTheta));
        auto bottomLeft = 0.5f
                          * glm::vec3(glm::sin(next_phi) * glm::cos(currentTheta),
                                      -glm::cos(next_phi),
                                      -glm::sin(next_phi) * glm::sin(currentTheta));
        auto topRight = 0.5f
                        * glm::vec3(glm::sin(phi) * glm::cos(nextTheta),
                                    -glm::cos(phi),
                                    -glm::sin(phi) * glm::sin(nextTheta));
        auto bottomRight = 0.5f
                           * glm::vec3(glm::sin(next_phi) * glm::cos(nextTheta),
                                       -glm::cos(next_phi),
                                       -glm::sin(next_phi) * glm::sin(nextTheta));
        makeTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Sphere::makeSphere() {
    // Task 7: create a full sphere using the makeWedge() function you
    //         implemented in Task 6
    // Note: think about how param 2 comes into play here!
    for (int i = 0; i < m_param2; i++) {
        float theta = i * 2 * M_PI / m_param2;
        float nextTheta = theta + 2 * M_PI / m_param2;
        makeWedge(theta, nextTheta);
    }
}

void Sphere::setVertexData() {
    // Uncomment these lines to make a wedge for Task 6, then comment them out for Task 7:

    // float thetaStep = glm::radians(360.f / m_param2);
    // float currentTheta = 0 * thetaStep;
    // float nextTheta = 1 * thetaStep;
    // makeWedge(currentTheta, nextTheta);

    // Uncomment these lines to make sphere for Task 7:
    if (m_param1 < 2) {
        m_param1 = 2;
    }
    if (m_param2 < 3) {
        m_param2 = 3;
    }
    makeSphere();
}
