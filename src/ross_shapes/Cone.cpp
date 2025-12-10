#include "Cone.h"
glm::vec3 Cone::calcNorm(glm::vec3 &pt)
{
    float xNorm = (2 * pt.x);
    float yNorm = -(1.f / 4.f) * (2.f * pt.y - 1.f);
    float zNorm = (2 * pt.z);

    return glm::normalize(glm::vec3{xNorm, yNorm, zNorm});
}

// Task 8: create function(s) to make tiles which you can call later on
// Note: Consider your makeTile() functions from Sphere and Cube
void Cone::makeCapTile(glm::vec3 topLeft,
                       glm::vec3 topRight,
                       glm::vec3 bottomLeft,
                       glm::vec3 bottomRight)
{
    auto v1 = topLeft;
    auto v2 = bottomRight;
    auto v3 = bottomLeft;
    insertVec3(m_vertexData, v1);
    insertVec3(m_vertexData, glm::vec3(0, -1, 0));
    insertVec3(m_vertexData, v2);
    insertVec3(m_vertexData, glm::vec3(0, -1, 0));
    insertVec3(m_vertexData, v3);
    insertVec3(m_vertexData, glm::vec3(0, -1, 0));
    v1 = bottomRight;
    v2 = topLeft;
    v3 = topRight;
    insertVec3(m_vertexData, v1);
    insertVec3(m_vertexData, glm::vec3(0, -1, 0));
    insertVec3(m_vertexData, v2);
    insertVec3(m_vertexData, glm::vec3(0, -1, 0));
    insertVec3(m_vertexData, v3);
    insertVec3(m_vertexData, glm::vec3(0, -1, 0));
}

void Cone::makeSlopeTile(glm::vec3 topLeft,
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

void Cone::makeTipTile(glm::vec3 topLeft,
                       glm::vec3 topRight,
                       glm::vec3 bottomLeft,
                       glm::vec3 bottomRight)
{
    auto v1 = topLeft;
    auto v2 = bottomLeft;
    auto v3 = bottomRight;
    insertVec3(m_vertexData, v1);
    insertVec3(m_vertexData, glm::normalize((calcNorm(v2) + calcNorm(v3)) * 0.5f));
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
    insertVec3(m_vertexData, glm::normalize((calcNorm(v1) + calcNorm(v2)) * 0.5f));
}
void Cone::makeCapSlice(float currentTheta, float nextTheta)
{
    // Task 8: create a slice of the cap face using your
    //         make tile function(s)
    // Note: think about how param 1 comes into play here!
    float y = -0.5f;
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
        makeCapTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Cone::makeSlopeSlice(float currentTheta, float nextTheta)
{
    // Task 9: create a single sloped face using your make
    //         tile function(s)
    // Note: think about how param 1 comes into play here!
    for (int i = 0; i < m_param1; i++) {
        float y = (float) i / m_param1 - 0.5;
        float next_y = (float) (i + 1) / m_param1 - 0.5;
        float r = 0.5 * (0.5 - y);
        float next_r = 0.5 * (0.5 - next_y);
        auto topLeft = glm::vec3(next_r * glm::cos(currentTheta),
                                 next_y,
                                 -next_r * glm::sin(currentTheta));
        auto bottomLeft = glm::vec3(r * glm::cos(currentTheta), y, -r * glm::sin(currentTheta));
        auto topRight = glm::vec3(next_r * glm::cos(nextTheta),
                                  next_y,
                                  -next_r * glm::sin(nextTheta));
        auto bottomRight = glm::vec3(r * glm::cos(nextTheta), y, -r * glm::sin(nextTheta));
        if (i == m_param1 - 1) {
            makeTipTile(topLeft, topRight, bottomLeft, bottomRight);
        } else {
            makeSlopeTile(topLeft, topRight, bottomLeft, bottomRight);
        }
    }
}

void Cone::makeWedge(float currentTheta, float nextTheta)
{
    // Task 10: create a single wedge of the Cone using the
    //          makeCapSlice() and makeSlopeSlice() functions you
    //          implemented in Task 5
    makeCapSlice(currentTheta, nextTheta);
    makeSlopeSlice(currentTheta, nextTheta);
}

void Cone::setVertexData()
{
    // Task 10: create a full cone using the makeWedge() function you
    //          just implemented
    // Note: think about how param 2 comes into play here!
    if (m_param2 < 3) {
        m_param2 = 3;
    }
    for (int i = 0; i < m_param2; i++) {
        float theta = i * 2 * M_PI / m_param2;
        float nextTheta = theta + 2 * M_PI / m_param2;
        makeWedge(theta, nextTheta);
    }
}
