#pragma once
#include "shapes/Shape.h"
#include <glm/glm.hpp>

class Cube : public Shape
{
private:
    void setVertexData();
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight,
                  float uTopLeft, float vTopLeft,
                  float uTopRight, float vTopRight,
                  float uBottomLeft, float vBottomLeft,
                  float uBottomRight, float vBottomRight);
    void makeFace(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
};
