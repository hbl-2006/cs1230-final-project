#pragma once
#include "shapes/Shape.h"
#include <glm/glm.hpp>

class Cone : public Shape
{
private:
    void setVertexData() override;
    glm::vec3 calcNorm(glm::vec3 &pt);
    void makeCapSlice(float currentTheta, float nextTheta);
    void makeSlopeSlice(float currentTheta, float nextTheta);
    void makeWedge(float currentTheta, float nextTheta);
    void makeConeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight,
                      float uTopLeft, float vTopLeft,
                      float uTopRight, float vTopRight,
                      float uBottomLeft, float vBottomLeft,
                      float uBottomRight, float vBottomRight);
    void makeCapTile(glm::vec3 topLeft,
                     glm::vec3 topRight,
                     glm::vec3 bottomLeft,
                     glm::vec3 bottomRight,
                     float uTopLeft, float vTopLeft,
                     float uTopRight, float vTopRight,
                     float uBottomLeft, float vBottomLeft,
                     float uBottomRight, float vBottomRight);
    void makeTip(float currentTheta, float nextTheta);
    float m_radius = 0.5;
};
