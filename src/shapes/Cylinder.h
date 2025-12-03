#pragma once

#include "shapes/Shape.h"
#include <glm/glm.hpp>

class Cylinder : public Shape
{
private:
    void setVertexData() override;
    glm::vec3 calcNorm(glm::vec3 &pt);
    void makeCapTile(glm::vec3 topLeft,
                     glm::vec3 topRight,
                     glm::vec3 bottomLeft,
                     glm::vec3 bottomRight,
                     bool top);
    void makeSideTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight);
    void makeCapSlice(float currentTheta, float nextTheta, bool top);
    void makeSideSlice(float currentTheta, float nextTheta);
    void makeWedge(float currentTheta, float nextTheta);
    float m_radius = 0.5;
};
