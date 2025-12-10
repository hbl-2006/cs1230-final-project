#include "Sphere.h"

void Sphere::makeTile(glm::vec3 topLeft, glm::vec3 topRight,
                      glm::vec3 bottomLeft, glm::vec3 bottomRight,
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

    glm::vec3 normalTL = glm::normalize(topLeft - origin);

    tangent = glm::normalize(tangent - normalTL * glm::dot(normalTL, tangent));
    if (glm::dot(glm::cross(normalTL, tangent), bitangent) < 0.0f) {
        tangent = tangent * -1.0f;
    }
    bitangent = glm::normalize(glm::cross(normalTL, tangent));

    // First triangle: topLeft, bottomLeft, bottomRight
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft - origin));
    m_vertexData.push_back(uTopLeft);
    m_vertexData.push_back(vTopLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(bottomLeft - origin));
    m_vertexData.push_back(uBottomLeft);
    m_vertexData.push_back(vBottomLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight - origin));
    m_vertexData.push_back(uBottomRight);
    m_vertexData.push_back(vBottomRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    // Second triangle: topLeft, bottomRight, topRight
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft - origin));
    m_vertexData.push_back(uTopLeft);
    m_vertexData.push_back(vTopLeft);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight - origin));
    m_vertexData.push_back(uBottomRight);
    m_vertexData.push_back(vBottomRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::normalize(topRight - origin));
    m_vertexData.push_back(uTopRight);
    m_vertexData.push_back(vTopRight);
    insertVec3(m_vertexData, tangent);
    insertVec3(m_vertexData, bitangent);
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    float delta = glm::radians(180.0f / m_param1);
    float r = 0.5f;

    for (int i = 0; i < m_param1; i++) {
        float phi = delta * i;
        float phiNext = delta * (i + 1);

        glm::vec3 topLeft = glm::vec3(
            r * glm::sin(phi) * glm::cos(currentTheta),
            r * glm::cos(phi),
            -r * glm::sin(phi) * glm::sin(currentTheta)
            );
        glm::vec3 topRight = glm::vec3(
            r * glm::sin(phi) * glm::cos(nextTheta),
            r * glm::cos(phi),
            -r * glm::sin(phi) * glm::sin(nextTheta)
            );
        glm::vec3 bottomLeft = glm::vec3(
            r * glm::sin(phiNext) * glm::cos(currentTheta),
            r * glm::cos(phiNext),
            -r * glm::sin(phiNext) * glm::sin(currentTheta)
            );
        glm::vec3 bottomRight = glm::vec3(
            r * glm::sin(phiNext) * glm::cos(nextTheta),
            r * glm::cos(phiNext),
            -r * glm::sin(phiNext) * glm::sin(nextTheta)
            );

        // Calculate UV coordinates
        auto calcUV = [](const glm::vec3& point) -> glm::vec2 {
            glm::vec3 p = glm::normalize(point);
            float phi = asin(p.y);
            float v = phi / M_PI + 0.5f;
            float theta = atan2(p.z, p.x);
            float u = 0.5f - (theta + M_PI) / (2.0f * M_PI);
            if (u < 0.0f) u += 1.0f;
            if (u > 1.0f) u -= 1.0f;
            // Handle poles
            if (abs(p.y - 0.5f) < 1e-4f || abs(p.y + 0.5f) < 1e-4f) {
                u = 0.5f;
            }
            return glm::vec2(u, v);
        };

        glm::vec2 uvTopLeft = calcUV(topLeft);
        glm::vec2 uvTopRight = calcUV(topRight);
        glm::vec2 uvBottomLeft = calcUV(bottomLeft);
        glm::vec2 uvBottomRight = calcUV(bottomRight);

        makeTile(topLeft, topRight, bottomLeft, bottomRight,
                 uvTopLeft.x, uvTopLeft.y,
                 uvTopRight.x, uvTopRight.y,
                 uvBottomLeft.x, uvBottomLeft.y,
                 uvBottomRight.x, uvBottomRight.y);
    }
}

void Sphere::makeSphere() {
    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWedge(currentTheta, nextTheta);
    }
}

void Sphere::setVertexData() {
    if (m_param1 < 2) {
        m_param1 = 2;
    }
    if (m_param2 < 3) {
        m_param2 = 3;
    }
    makeSphere();
}
