#pragma once

#include "shapes/Shape.h"
#include <vector>
#include <glm/glm.hpp>

class Sphere : public Shape
{
private:
    void setVertexData() override;
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeWedge(float currTheta, float nextTheta);
    void makeSphere();
    float m_radius = 0.5;
};
