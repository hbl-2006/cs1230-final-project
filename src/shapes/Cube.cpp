#include "Cube.h"

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight,
                    float uTopLeft, float vTopLeft,
                    float uTopRight, float vTopRight,
                    float uBottomLeft, float vBottomLeft,
                    float uBottomRight, float vBottomRight) {
    // Calculate face normal
    glm::vec3 edge1 = bottomLeft - topLeft;
    glm::vec3 edge2 = topRight - topLeft;
    glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

    // Calculate tangent and bitangent
    glm::vec3 deltaPos1 = topRight - topLeft;
    glm::vec3 deltaPos2 = bottomLeft - topLeft;
    glm::vec2 deltaUV1 = glm::vec2(uTopRight - uTopLeft, vTopRight - vTopLeft);
    glm::vec2 deltaUV2 = glm::vec2(uBottomLeft - uTopLeft, vBottomLeft - vTopLeft);

    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x + 0.0001f);
    glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
    glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

    tangent = glm::normalize(tangent - normal * glm::dot(normal, tangent));
    if (glm::dot(glm::cross(normal, tangent), bitangent) < 0.0f) {
        tangent = tangent * -1.0f;
    }
    bitangent = glm::normalize(glm::cross(normal, tangent));

    // First triangle: topLeft, bottomLeft, bottomRight
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal);
    m_vertexData.push_back(uTopLeft);
    m_vertexData.push_back(vTopLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal);
    m_vertexData.push_back(uBottomLeft);
    m_vertexData.push_back(vBottomLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal);
    m_vertexData.push_back(uBottomRight);
    m_vertexData.push_back(vBottomRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    // Second triangle: topLeft, bottomRight, topRight
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal);
    m_vertexData.push_back(uTopLeft);
    m_vertexData.push_back(vTopLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal);
    m_vertexData.push_back(uBottomRight);
    m_vertexData.push_back(vBottomRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal);
    m_vertexData.push_back(uTopRight);
    m_vertexData.push_back(vTopRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    for (int row = 0; row < m_param1; row++) {
        for (int col = 0; col < m_param1; col++) {
            float rowStart = (float)row / m_param1;
            float rowEnd = (float)(row + 1) / m_param1;
            float colStart = (float)col / m_param1;
            float colEnd = (float)(col + 1) / m_param1;

            // Calculate tile corners
            glm::vec3 tileTopLeft = topLeft +
                                    colStart * (topRight - topLeft) +
                                    rowStart * (bottomLeft - topLeft);

            glm::vec3 tileTopRight = topLeft +
                                     colEnd * (topRight - topLeft) +
                                     rowStart * (bottomLeft - topLeft);

            glm::vec3 tileBottomLeft = topLeft +
                                       colStart * (topRight - topLeft) +
                                       rowEnd * (bottomLeft - topLeft);

            glm::vec3 tileBottomRight = topLeft +
                                        colEnd * (topRight - topLeft) +
                                        rowEnd * (bottomLeft - topLeft);

            // Calculate UV coordinates
            float uTopLeft = colStart;
            float vTopLeft = 1.0f - rowStart;
            float uTopRight = colEnd;
            float vTopRight = 1.0f - rowStart;
            float uBottomLeft = colStart;
            float vBottomLeft = 1.0f - rowEnd;
            float uBottomRight = colEnd;
            float vBottomRight = 1.0f - rowEnd;

            makeTile(tileTopLeft, tileTopRight, tileBottomLeft, tileBottomRight,
                     uTopLeft, vTopLeft, uTopRight, vTopRight,
                     uBottomLeft, vBottomLeft, uBottomRight, vBottomRight);
        }
    }
}

void Cube::setVertexData() {
    // Front face (+Z)
    makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
             glm::vec3( 0.5f,  0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3( 0.5f, -0.5f, 0.5f));

    // Top face (+Y)
    makeFace(glm::vec3(-0.5f, 0.5f, -0.5f),
             glm::vec3( 0.5f, 0.5f, -0.5f),
             glm::vec3(-0.5f, 0.5f,  0.5f),
             glm::vec3( 0.5f, 0.5f,  0.5f));

    // Bottom face (-Y)
    makeFace(glm::vec3(-0.5f, -0.5f,  0.5f),
             glm::vec3( 0.5f, -0.5f,  0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f));

    // Back face (-Z)
    makeFace(glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f));

    // Right face (+X)
    makeFace(glm::vec3( 0.5f,  0.5f,  0.5f),
             glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f,  0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f));

    // Left face (-X)
    makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f,  0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f,  0.5f));
}
