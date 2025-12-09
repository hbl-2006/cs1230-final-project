#include "Cone.h"

glm::vec3 Cone::calcNorm(glm::vec3& pt) {
    float xNorm = (2 * pt.x);
    float yNorm = -(1.f/4.f) * (2.f * pt.y - 1.f);
    float zNorm = (2 * pt.z);
    return glm::normalize(glm::vec3{ xNorm, yNorm, zNorm });
}

void Cone::makeConeTile(glm::vec3 topLeft,
                        glm::vec3 topRight,
                        glm::vec3 bottomLeft,
                        glm::vec3 bottomRight,
                        float uTopLeft, float vTopLeft,
                        float uTopRight, float vTopRight,
                        float uBottomLeft, float vBottomLeft,
                        float uBottomRight, float vBottomRight) {
    // Calculate tangent and bitangent
    glm::vec3 deltaPos1 = topRight - topLeft;
    glm::vec3 deltaPos2 = bottomLeft - topLeft;
    glm::vec2 deltaUV1 = glm::vec2(uTopRight - uTopLeft, vTopRight - vTopLeft);
    glm::vec2 deltaUV2 = glm::vec2(uBottomLeft - uTopLeft, vBottomLeft - vTopLeft);

    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x + 0.0001f);
    glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
    glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

    glm::vec3 normalTL = calcNorm(topLeft);

    tangent = glm::normalize(tangent - normalTL * glm::dot(normalTL, tangent));
    if (glm::dot(glm::cross(normalTL, tangent), bitangent) < 0.0f) {
        tangent = tangent * -1.0f;
    }
    bitangent = glm::normalize(glm::cross(normalTL, tangent));

    // First triangle
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, calcNorm(topLeft));
    m_vertexData.push_back(uTopLeft);
    m_vertexData.push_back(vTopLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, calcNorm(bottomLeft));
    m_vertexData.push_back(uBottomLeft);
    m_vertexData.push_back(vBottomLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, calcNorm(bottomRight));
    m_vertexData.push_back(uBottomRight);
    m_vertexData.push_back(vBottomRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    // Second triangle
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, calcNorm(topLeft));
    m_vertexData.push_back(uTopLeft);
    m_vertexData.push_back(vTopLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, calcNorm(bottomRight));
    m_vertexData.push_back(uBottomRight);
    m_vertexData.push_back(vBottomRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, calcNorm(topRight));
    m_vertexData.push_back(uTopRight);
    m_vertexData.push_back(vTopRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);
}

void Cone::makeCapTile(glm::vec3 topLeft,
                       glm::vec3 topRight,
                       glm::vec3 bottomLeft,
                       glm::vec3 bottomRight,
                       float uTopLeft, float vTopLeft,
                       float uTopRight, float vTopRight,
                       float uBottomLeft, float vBottomLeft,
                       float uBottomRight, float vBottomRight) {
    glm::vec3 normal = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 bitangent = glm::vec3(0.0f, 0.0f, 1.0f);

    // First triangle
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

    // Second triangle
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

void Cone::makeCapSlice(float currentTheta, float nextTheta) {
    float delta = 0.5f / m_param1;
    float y = -0.5f;

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

        makeCapTile(topLeft, topRight, bottomLeft, bottomRight,
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

        makeCapTile(topLeft, topRight, bottomLeft, bottomRight,
                    uTopLeft, vTopLeft, uTopRight, vTopRight,
                    uBottomLeft, vBottomLeft, uBottomRight, vBottomRight);
    }
}

void Cone::makeTip(float currentTheta, float nextTheta) {
    float base_r = 0.5f;
    float next_r = base_r / m_param1;
    float delta = 1.0f / m_param1;
    float y1 = 0.5f;
    float y2 = y1 - delta;

    float y = -0.5f;
    float x1 = glm::cos(currentTheta);
    float z1 = glm::sin(currentTheta);
    float x2 = glm::cos(nextTheta);
    float z2 = glm::sin(nextTheta);

    glm::vec3 v1 = glm::vec3(base_r * x1, y, base_r * z1);
    glm::vec3 v2 = glm::vec3(base_r * x2, y, base_r * z2);
    glm::vec3 n1 = calcNorm(v1);
    glm::vec3 n2 = calcNorm(v2);

    glm::vec3 topRight = glm::vec3(0.0f, y1, 0.0f);
    glm::vec3 topLeft = glm::vec3(0.0f, y1, 0.0f);
    glm::vec3 bottomRight = glm::vec3(next_r * x1, y2, next_r * z1);
    glm::vec3 bottomLeft = glm::vec3(next_r * x2, y2, next_r * z2);

    glm::vec3 n = (n1 + n2) * 0.5f;

    float u1 = 1.0f - currentTheta / (2.0f * M_PI);
    float u2 = 1.0f - nextTheta / (2.0f * M_PI);
    float uTip = 0.5f;
    float vTip = 1.0f;
    float vBottom = 1.0f - delta;

    // Calculate tangent and bitangent
    glm::vec3 deltaPos1 = bottomRight - topLeft;
    glm::vec3 deltaPos2 = bottomLeft - topLeft;
    glm::vec2 deltaUV1 = glm::vec2(u1 - uTip, vBottom - vTip);
    glm::vec2 deltaUV2 = glm::vec2(u2 - uTip, vBottom - vTip);

    float r_val = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x + 0.0001f);
    glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r_val;
    glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r_val;

    tangent = glm::normalize(tangent - n * glm::dot(n, tangent));
    if (glm::dot(glm::cross(n, tangent), bitangent) < 0.0f) {
        tangent = tangent * -1.0f;
    }
    bitangent = glm::normalize(glm::cross(n, tangent));

    // First triangle
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, n);
    m_vertexData.push_back(uTip);
    m_vertexData.push_back(vTip);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, calcNorm(bottomLeft));
    m_vertexData.push_back(u2);
    m_vertexData.push_back(vBottom);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, calcNorm(bottomRight));
    m_vertexData.push_back(u1);
    m_vertexData.push_back(vBottom);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    // Second triangle
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, n);
    m_vertexData.push_back(uTip);
    m_vertexData.push_back(vTip);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, calcNorm(bottomRight));
    m_vertexData.push_back(u1);
    m_vertexData.push_back(vBottom);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, n);
    m_vertexData.push_back(uTip);
    m_vertexData.push_back(vTip);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);
}

void Cone::makeSlopeSlice(float currentTheta, float nextTheta) {
    float delta = 1.0f / m_param1;
    float y1 = 0.5f;
    float y2;
    float delta_r = 0.5f / m_param1;

    makeTip(currentTheta, nextTheta);
    y1 -= delta;
    y2 = y1 - delta;

    for (int i = 1; i < m_param1; i++) {
        float r = delta_r * i;
        float next_r = r + delta_r;

        float x1 = glm::cos(currentTheta);
        float z1 = glm::sin(currentTheta);
        float x2 = glm::cos(nextTheta);
        float z2 = glm::sin(nextTheta);

        glm::vec3 topRight = glm::vec3(r * x1, y1, r * z1);
        glm::vec3 topLeft = glm::vec3(r * x2, y1, r * z2);
        glm::vec3 bottomRight = glm::vec3(next_r * x1, y2, next_r * z1);
        glm::vec3 bottomLeft = glm::vec3(next_r * x2, y2, next_r * z2);

        float u1 = 1.0f - currentTheta / (2.0f * M_PI);
        float u2 = 1.0f - nextTheta / (2.0f * M_PI);
        float v1 = 1.0f - (float)i / m_param1;
        float v2 = 1.0f - (float)(i + 1) / m_param1;

        makeConeTile(topLeft, topRight, bottomLeft, bottomRight,
                     u2, v1, u1, v1, u2, v2, u1, v2);

        y1 -= delta;
        y2 = y1 - delta;
    }
}

void Cone::makeWedge(float currentTheta, float nextTheta) {
    makeCapSlice(currentTheta, nextTheta);
    makeSlopeSlice(currentTheta, nextTheta);
}

void Cone::setVertexData() {
    if (m_param2 < 3) {
        m_param2 = 3;
    }
    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWedge(currentTheta, nextTheta);
    }
}
