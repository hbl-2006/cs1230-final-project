#include "Cylinder.h"

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

static inline glm::vec3 zeroYNorm(glm::vec3 v, glm::vec3 o) {
    glm::vec3 n = glm::normalize(v - o);
    n.y = 0.0f;
    return n;
}

void Cylinder::makeBottomCapSlice(float currentTheta, float nextTheta) {
    float delta = 0.5f / m_param1;
    float y = 0.5f;
    glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 bitangent = glm::vec3(0.0f, 0.0f, -1.0f);

    for (int i = 0; i < m_param1; i++) {
        float r = delta * i;
        float next_r = r + delta;
        float x1 = glm::cos(currentTheta);
        float z1 = glm::sin(currentTheta);
        float x2 = glm::cos(nextTheta);
        float z2 = glm::sin(nextTheta);

        glm::vec3 topLeft = glm::vec3(r * x1, y, r * z1);
        glm::vec3 topRight = topLeft;
        glm::vec3 bottomLeft = glm::vec3(next_r * x1, y, next_r * z1);
        glm::vec3 bottomRight = glm::vec3(next_r * x2, y, next_r * z2);

        float uTopLeft = topLeft.x + 0.5f;
        float vTopLeft = -topLeft.z + 0.5f;
        float uTopRight = uTopLeft;
        float vTopRight = vTopLeft;
        float uBottomLeft = bottomLeft.x + 0.5f;
        float vBottomLeft = -bottomLeft.z + 0.5f;
        float uBottomRight = bottomRight.x + 0.5f;
        float vBottomRight = -bottomRight.z + 0.5f;

        insertVec3(m_vertexData, topRight);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uTopRight);
        m_vertexData.push_back(vTopRight);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);

        insertVec3(m_vertexData, bottomRight);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uBottomRight);
        m_vertexData.push_back(vBottomRight);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);

        insertVec3(m_vertexData, bottomLeft);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uBottomLeft);
        m_vertexData.push_back(vBottomLeft);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);

        insertVec3(m_vertexData, topRight);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uTopRight);
        m_vertexData.push_back(vTopRight);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);

        insertVec3(m_vertexData, bottomLeft);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uBottomLeft);
        m_vertexData.push_back(vBottomLeft);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);

        insertVec3(m_vertexData, topLeft);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uTopLeft);
        m_vertexData.push_back(vTopLeft);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);
    }

    for (int i = 1; i < m_param1; i++) {
        float r = delta * i;
        float next_r = r + delta;
        float x1 = glm::cos(currentTheta);
        float z1 = glm::sin(currentTheta);
        float x2 = glm::cos(nextTheta);
        float z2 = glm::sin(nextTheta);

        glm::vec3 topLeft = glm::vec3(r * x1, y, r * z1);
        glm::vec3 topRight = topLeft;
        glm::vec3 bottomLeft = glm::vec3(next_r * x2, y, next_r * z2);
        glm::vec3 bottomRight = glm::vec3(r * x2, y, r * z2);

        float uTopLeft = topLeft.x + 0.5f;
        float vTopLeft = -topLeft.z + 0.5f;
        float uTopRight = uTopLeft;
        float vTopRight = vTopLeft;
        float uBottomLeft = bottomLeft.x + 0.5f;
        float vBottomLeft = -bottomLeft.z + 0.5f;
        float uBottomRight = bottomRight.x + 0.5f;
        float vBottomRight = -bottomRight.z + 0.5f;

        insertVec3(m_vertexData, topRight);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uTopRight);
        m_vertexData.push_back(vTopRight);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);

        insertVec3(m_vertexData, bottomRight);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uBottomRight);
        m_vertexData.push_back(vBottomRight);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);

        insertVec3(m_vertexData, bottomLeft);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uBottomLeft);
        m_vertexData.push_back(vBottomLeft);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);

        insertVec3(m_vertexData, topRight);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uTopRight);
        m_vertexData.push_back(vTopRight);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);

        insertVec3(m_vertexData, bottomLeft);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uBottomLeft);
        m_vertexData.push_back(vBottomLeft);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);

        insertVec3(m_vertexData, topLeft);
        insertVec3(m_vertexData, normal);
        m_vertexData.push_back(uTopLeft);
        m_vertexData.push_back(vTopLeft);
        insertVec3(m_vertexData, tangent);
        insertVec3(m_vertexData, bitangent);
    }
}

void Cylinder::makeCapTile(glm::vec3 topLeft,
                           glm::vec3 topRight,
                           glm::vec3 bottomLeft,
                           glm::vec3 bottomRight,
                           glm::vec3 normal,
                           float uTopLeft, float vTopLeft,
                           float uTopRight, float vTopRight,
                           float uBottomLeft, float vBottomLeft,
                           float uBottomRight, float vBottomRight) {
    glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 bitangent = glm::vec3(0.0f, 0.0f, 1.0f);

    if (normal.y < 0.0f) {
        bitangent = glm::vec3(0.0f, 0.0f, -1.0f);
    }

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

void Cylinder::makeSideTile(glm::vec3 topLeft,
                            glm::vec3 topRight,
                            glm::vec3 bottomLeft,
                            glm::vec3 bottomRight,
                            float uTopLeft, float vTopLeft,
                            float uTopRight, float vTopRight,
                            float uBottomLeft, float vBottomLeft,
                            float uBottomRight, float vBottomRight) {
    glm::vec3 origin = glm::vec3(0.0f);

    // Calculate tangent and bitangent
    glm::vec3 deltaPos1 = topRight - topLeft;
    glm::vec3 deltaPos2 = bottomLeft - topLeft;
    glm::vec2 deltaUV1 = glm::vec2(uTopRight - uTopLeft, vTopRight - vTopLeft);
    glm::vec2 deltaUV2 = glm::vec2(uBottomLeft - uTopLeft, vBottomLeft - vTopLeft);

    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x + 0.0001f);
    glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
    glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

    glm::vec3 normalTL = zeroYNorm(topLeft, origin);

    // Gram-Schmidt orthogonalization
    tangent = glm::normalize(tangent - normalTL * glm::dot(normalTL, tangent));
    if (glm::dot(glm::cross(normalTL, tangent), bitangent) < 0.0f) {
        tangent = tangent * -1.0f;
    }
    bitangent = glm::normalize(glm::cross(normalTL, tangent));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, zeroYNorm(topLeft, origin));
    m_vertexData.push_back(uTopLeft);
    m_vertexData.push_back(vTopLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, zeroYNorm(bottomLeft, origin));
    m_vertexData.push_back(uBottomLeft);
    m_vertexData.push_back(vBottomLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, zeroYNorm(bottomRight, origin));
    m_vertexData.push_back(uBottomRight);
    m_vertexData.push_back(vBottomRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, zeroYNorm(topLeft, origin));
    m_vertexData.push_back(uTopLeft);
    m_vertexData.push_back(vTopLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, zeroYNorm(bottomRight, origin));
    m_vertexData.push_back(uBottomRight);
    m_vertexData.push_back(vBottomRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, zeroYNorm(topRight, origin));
    m_vertexData.push_back(uTopRight);
    m_vertexData.push_back(vTopRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);
}

void Cylinder::makeTopCapSlice(float currentTheta, float nextTheta) {
    float delta = 0.5f / m_param1;
    float y = -0.5f;
    glm::vec3 normal = glm::vec3(0.0f, -1.0f, 0.0f);

    for (int i = 0; i < m_param1; i++) {
        float r = delta * i;
        float next_r = r + delta;
        float x1 = glm::cos(currentTheta);
        float z1 = glm::sin(currentTheta);
        float x2 = glm::cos(nextTheta);
        float z2 = glm::sin(nextTheta);

        glm::vec3 topLeft = glm::vec3(r * x1, y, r * z1);
        glm::vec3 topRight = topLeft;
        glm::vec3 bottomLeft = glm::vec3(next_r * x1, y, next_r * z1);
        glm::vec3 bottomRight = glm::vec3(next_r * x2, y, next_r * z2);

        float uTopLeft = topLeft.x + 0.5f;
        float vTopLeft = topLeft.z + 0.5f;
        float uTopRight = uTopLeft;
        float vTopRight = vTopLeft;
        float uBottomLeft = bottomLeft.x + 0.5f;
        float vBottomLeft = bottomLeft.z + 0.5f;
        float uBottomRight = bottomRight.x + 0.5f;
        float vBottomRight = bottomRight.z + 0.5f;

        makeCapTile(topLeft, topRight, bottomLeft, bottomRight, normal,
                    uTopLeft, vTopLeft, uTopRight, vTopRight,
                    uBottomLeft, vBottomLeft, uBottomRight, vBottomRight);
    }

    for (int i = 1; i < m_param1; i++) {
        float r = delta * i;
        float next_r = r + delta;
        float x1 = glm::cos(currentTheta);
        float z1 = glm::sin(currentTheta);
        float x2 = glm::cos(nextTheta);
        float z2 = glm::sin(nextTheta);

        glm::vec3 topLeft = glm::vec3(r * x1, y, r * z1);
        glm::vec3 topRight = topLeft;
        glm::vec3 bottomLeft = glm::vec3(next_r * x2, y, next_r * z2);
        glm::vec3 bottomRight = glm::vec3(r * x2, y, r * z2);

        float uTopLeft = topLeft.x + 0.5f;
        float vTopLeft = topLeft.z + 0.5f;
        float uTopRight = uTopLeft;
        float vTopRight = vTopLeft;
        float uBottomLeft = bottomLeft.x + 0.5f;
        float vBottomLeft = bottomLeft.z + 0.5f;
        float uBottomRight = bottomRight.x + 0.5f;
        float vBottomRight = bottomRight.z + 0.5f;

        makeCapTile(topLeft, topRight, bottomLeft, bottomRight, normal,
                    uTopLeft, vTopLeft, uTopRight, vTopRight,
                    uBottomLeft, vBottomLeft, uBottomRight, vBottomRight);
    }
}

void Cylinder::makeSideSlice(float currentTheta, float nextTheta) {
    float delta = 1.0f / m_param1;
    float y1 = 0.5f;
    float y2 = y1 - delta;
    float r = 0.5f;

    for (int i = 0; i < m_param1; i++) {
        float x1 = glm::cos(currentTheta);
        float z1 = glm::sin(currentTheta);
        float x2 = glm::cos(nextTheta);
        float z2 = glm::sin(nextTheta);

        glm::vec3 topRight = glm::vec3(r * x1, y1, r * z1);
        glm::vec3 topLeft = glm::vec3(r * x2, y1, r * z2);
        glm::vec3 bottomRight = glm::vec3(r * x1, y2, r * z1);
        glm::vec3 bottomLeft = glm::vec3(r * x2, y2, r * z2);

        float u1 = 1.0f - currentTheta / (2.0f * M_PI);
        float u2 = 1.0f - nextTheta / (2.0f * M_PI);
        float v1 = 1.0f - (float)i / m_param1;
        float v2 = 1.0f - (float)(i + 1) / m_param1;

        makeSideTile(topLeft, topRight, bottomLeft, bottomRight,
                     u2, v1, u1, v1, u2, v2, u1, v2);

        y1 -= delta;
        y2 = y1 - delta;
    }
}

void Cylinder::makeWedge(float currentTheta, float nextTheta) {
    makeTopCapSlice(currentTheta, nextTheta);
    makeBottomCapSlice(currentTheta, nextTheta);
    makeSideSlice(currentTheta, nextTheta);
}

void Cylinder::setVertexData() {
    float thetaStep = glm::radians(360.f / m_param2);

    for (int i = 0; i < m_param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWedge(currentTheta, nextTheta);
    }
}

void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
