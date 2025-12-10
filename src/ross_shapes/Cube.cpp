#include "Cube.h"

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.
    auto v1 = topLeft;
    auto v2 = bottomLeft;
    auto v3 = bottomRight;
    insertVec3(m_vertexData, v1);
    insertVec3(m_vertexData, glm::normalize(glm::cross(v2 - v1, v3 - v1)));
    insertVec3(m_vertexData, v2);
    insertVec3(m_vertexData, glm::normalize(glm::cross(v3 - v2, v1 - v2)));
    insertVec3(m_vertexData, v3);
    insertVec3(m_vertexData, glm::normalize(glm::cross(v1 - v3, v2 - v3)));
    v1 = bottomRight;
    v2 = topRight;
    v3 = topLeft;
    insertVec3(m_vertexData, v1);
    insertVec3(m_vertexData, glm::normalize(glm::cross(v2 - v1, v3 - v1)));
    insertVec3(m_vertexData, v2);
    insertVec3(m_vertexData, glm::normalize(glm::cross(v3 - v2, v1 - v2)));
    insertVec3(m_vertexData, v3);
    insertVec3(m_vertexData, glm::normalize(glm::cross(v1 - v3, v2 - v3)));
}

void Cube::makeXFace(glm::vec3 topLeft,
                     glm::vec3 topRight,
                     glm::vec3 bottomLeft,
                     glm::vec3 bottomRight)
{
    auto z_dist = topRight.z - topLeft.z;
    auto y_dist = bottomLeft.y - topLeft.y;
    for (int y = 0; y < m_param1; y++) {
        for (int z = 0; z < m_param1; z++) {
            auto newTopLeft = glm::vec3(topLeft.x,
                                        topLeft.y + y * y_dist / m_param1,
                                        topLeft.z + z * z_dist / m_param1);
            auto newTopRight = glm::vec3(newTopLeft.x,
                                         newTopLeft.y,
                                         newTopLeft.z + z_dist / m_param1);
            auto newBottomLeft = glm::vec3(newTopLeft.x,
                                           newTopLeft.y + y_dist / m_param1,
                                           newTopLeft.z);
            auto newBottomRight = glm::vec3(newTopLeft.x,
                                            newTopLeft.y + y_dist / m_param1,
                                            newTopLeft.z + z_dist / m_param1);
            makeTile(newTopLeft, newTopRight, newBottomLeft, newBottomRight);
        }
    }
}

void Cube::makeYFace(glm::vec3 topLeft,
                     glm::vec3 topRight,
                     glm::vec3 bottomLeft,
                     glm::vec3 bottomRight)
{
    auto x_dist = topRight.x - topLeft.x;
    auto z_dist = bottomLeft.z - topLeft.z;
    for (int z = 0; z < m_param1; z++) {
        for (int x = 0; x < m_param1; x++) {
            auto newTopLeft = glm::vec3(topLeft.x + x * x_dist / m_param1,
                                        topLeft.y,
                                        topLeft.z + z * z_dist / m_param1);
            auto newTopRight = glm::vec3(newTopLeft.x + x_dist / m_param1,
                                         newTopLeft.y,
                                         newTopLeft.z);
            auto newBottomLeft = glm::vec3(newTopLeft.x,
                                           newTopLeft.y,
                                           newTopLeft.z + z_dist / m_param1);
            auto newBottomRight = glm::vec3(newTopLeft.x + x_dist / m_param1,
                                            newTopLeft.y,
                                            newTopLeft.z + z_dist / m_param1);
            makeTile(newTopLeft, newTopRight, newBottomLeft, newBottomRight);
        }
    }
}

void Cube::makeZFace(glm::vec3 topLeft,
                     glm::vec3 topRight,
                     glm::vec3 bottomLeft,
                     glm::vec3 bottomRight)
{
    auto x_dist = topRight.x - topLeft.x;
    auto y_dist = bottomLeft.y - topLeft.y;
    for (int y = 0; y < m_param1; y++) {
        for (int x = 0; x < m_param1; x++) {
            auto newTopLeft = glm::vec3(topLeft.x + x * x_dist / m_param1,
                                        topLeft.y + y * y_dist / m_param1,
                                        topLeft.z);
            auto newTopRight = glm::vec3(newTopLeft.x + x_dist / m_param1,
                                         newTopLeft.y,
                                         newTopLeft.z);
            auto newBottomLeft = glm::vec3(newTopLeft.x,
                                           newTopLeft.y + y_dist / m_param1,
                                           newTopLeft.z);
            auto newBottomRight = glm::vec3(newTopLeft.x + x_dist / m_param1,
                                            newTopLeft.y + y_dist / m_param1,
                                            newTopLeft.z);
            makeTile(newTopLeft, newTopRight, newBottomLeft, newBottomRight);
        }
    }
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 3: create a single side of the cube out of the 4
    //         given points and makeTile()
    // Note: think about how param 1 affects the number of triangles on
    //       the face of the cube
    auto vecDiff = bottomRight - topLeft;
    // Figure out which plane the face is coplanar to:
    if (vecDiff.x == 0) {
        makeXFace(topLeft, topRight, bottomLeft, bottomRight);
    } else if (vecDiff.y == 0) {
        makeYFace(topLeft, topRight, bottomLeft, bottomRight);
    } else if (vecDiff.z == 0) {
        makeZFace(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Cube::setVertexData() {
    // Uncomment these lines for Task 2, then comment them out for Task 3:

    // makeTile(glm::vec3(-0.5f, 0.5f, 0.5f),
    //          glm::vec3(0.5f, 0.5f, 0.5f),
    //          glm::vec3(-0.5f, -0.5f, 0.5f),
    //          glm::vec3(0.5f, -0.5f, 0.5f));

    // Uncomment these lines for Task 3:

    // makeFace(glm::vec3(-0.5f, 0.5f, 0.5f),
    //          glm::vec3(0.5f, 0.5f, 0.5f),
    //          glm::vec3(-0.5f, -0.5f, 0.5f),
    //          glm::vec3(0.5f, -0.5f, 0.5f));

    // Task 4: Use the makeFace() function to make all 6 sides of the cube
    auto v1 = glm::vec3(-0.5f, 0.5f, 0.5f);
    auto v2 = glm::vec3(0.5f, 0.5f, 0.5f);
    auto v3 = glm::vec3(-0.5f, -0.5f, 0.5f);
    auto v4 = glm::vec3(0.5f, -0.5f, 0.5f);
    auto v5 = glm::vec3(-0.5f, 0.5f, -0.5f);
    auto v6 = glm::vec3(0.5f, 0.5f, -0.5f);
    auto v7 = glm::vec3(-0.5f, -0.5f, -0.5f);
    auto v8 = glm::vec3(0.5f, -0.5f, -0.5f);

    makeFace(v1, v2, v3, v4);
    makeFace(v6, v5, v8, v7);
    makeFace(v5, v6, v1, v2);
    makeFace(v2, v6, v4, v8);
    makeFace(v5, v1, v7, v3);
    makeFace(v3, v4, v7, v8);
}
