#include "Cylinder.h"

glm::vec3 Cylinder::calcNorm(glm::vec3 &pt)
{
    float xNorm = (2 * pt.x);
    float yNorm = 0;
    float zNorm = (2 * pt.z);

    return glm::normalize(glm::vec3{xNorm, yNorm, zNorm});
}

void Cylinder::makeCapTile(
    glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, bool top)
{
    int normal = top ? 1 : -1;
    auto v1 = topLeft;
    auto v2 = bottomRight;
    auto v3 = bottomLeft;
    if (top) {
        v2 = bottomLeft;
        v3 = bottomRight;
    }
    insertVec3(m_vertexData, v1);
    insertVec3(m_vertexData, glm::vec3(0, normal, 0));
    insertVec3(m_vertexData, v2);
    insertVec3(m_vertexData, glm::vec3(0, normal, 0));
    insertVec3(m_vertexData, v3);
    insertVec3(m_vertexData, glm::vec3(0, normal, 0));
    v1 = bottomRight;
    v2 = topLeft;
    v3 = topRight;
    if (top) {
        v2 = topRight;
        v3 = topLeft;
    }
    insertVec3(m_vertexData, v1);
    insertVec3(m_vertexData, glm::vec3(0, normal, 0));
    insertVec3(m_vertexData, v2);
    insertVec3(m_vertexData, glm::vec3(0, normal, 0));
    insertVec3(m_vertexData, v3);
    insertVec3(m_vertexData, glm::vec3(0, normal, 0));
}

void Cylinder::makeSideTile(glm::vec3 topLeft,
                            glm::vec3 topRight,
                            glm::vec3 bottomLeft,
                            glm::vec3 bottomRight)
{
    auto v1 = topLeft;
    auto v2 = bottomLeft;
    auto v3 = bottomRight;
    insertVec3(m_vertexData, v1);
    insertVec3(m_vertexData, calcNorm(v1));
    insertVec3(m_vertexData, v2);
    insertVec3(m_vertexData, calcNorm(v2));
    insertVec3(m_vertexData, v3);
    insertVec3(m_vertexData, calcNorm(v3));
    v1 = bottomRight;
    v2 = topRight;
    v3 = topLeft;
    insertVec3(m_vertexData, v1);
    insertVec3(m_vertexData, calcNorm(v1));
    insertVec3(m_vertexData, v2);
    insertVec3(m_vertexData, calcNorm(v2));
    insertVec3(m_vertexData, v3);
    insertVec3(m_vertexData, calcNorm(v3));
}

void Cylinder::makeCapSlice(float currentTheta, float nextTheta, bool top)
{
    // Task 8: create a slice of the cap face using your
    //         make tile function(s)
    // Note: think about how param 1 comes into play here!
    float y = top ? 0.5f : -0.5f;
    float r = 0.5f;
    for (int i = 0; i < m_param1; i++) {
        // each slice is a concentric "circle" of tiles, so we calc the radial difference
        float inner_r = r * i / m_param1;
        float outer_r = r * (i + 1) / m_param1;
        auto topLeft = glm::vec3(inner_r * glm::cos(currentTheta),
                                 y,
                                 -inner_r * glm::sin(currentTheta));
        auto bottomLeft = glm::vec3(outer_r * glm::cos(currentTheta),
                                    y,
                                    -outer_r * glm::sin(currentTheta));
        auto topRight = glm::vec3(inner_r * glm::cos(nextTheta), y, -inner_r * glm::sin(nextTheta));
        auto bottomRight = glm::vec3(outer_r * glm::cos(nextTheta),
                                     y,
                                     -outer_r * glm::sin(nextTheta));
        makeCapTile(topLeft, topRight, bottomLeft, bottomRight, top);
    }
}

void Cylinder::makeSideSlice(float currentTheta, float nextTheta)
{
    // Note: think about how param 1 comes into play here!
    for (int i = 0; i < m_param1; i++) {
        float y = (float) i / m_param1 - 0.5;
        float next_y = (float) (i + 1) / m_param1 - 0.5;
        float r = 0.5;
        auto topLeft = glm::vec3(r * glm::cos(currentTheta), next_y, -r * glm::sin(currentTheta));
        auto bottomLeft = glm::vec3(r * glm::cos(currentTheta), y, -r * glm::sin(currentTheta));
        auto topRight = glm::vec3(r * glm::cos(nextTheta), next_y, -r * glm::sin(nextTheta));
        auto bottomRight = glm::vec3(r * glm::cos(nextTheta), y, -r * glm::sin(nextTheta));

        makeSideTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Cylinder::makeWedge(float currentTheta, float nextTheta)
{
    // Task 10: create a single wedge of the Cone using the
    //          makeCapSlice() and makeSlopeSlice() functions you
    //          implemented in Task 5
    makeCapSlice(currentTheta, nextTheta, false);
    makeSideSlice(currentTheta, nextTheta);
    makeCapSlice(currentTheta, nextTheta, true);
}

void Cylinder::setVertexData() {
    if (m_param2 < 3) {
        m_param2 = 3;
    }
    // TODO for Project 5: Lights, Camera
    for (int i = 0; i < m_param2; i++) {
        float theta = i * 2 * M_PI / m_param2;
        float nextTheta = theta + 2 * M_PI / m_param2;
        makeWedge(theta, nextTheta);
    }
}
