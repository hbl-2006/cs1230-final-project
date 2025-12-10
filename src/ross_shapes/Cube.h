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
                  glm::vec3 bottomRight);
    void makeFace(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeXFace(glm::vec3 topLeft,
                   glm::vec3 topRight,
                   glm::vec3 bottomLeft,
                   glm::vec3 bottomRight);
    void makeYFace(glm::vec3 topLeft,
                   glm::vec3 topRight,
                   glm::vec3 bottomLeft,
                   glm::vec3 bottomRight);
    void makeZFace(glm::vec3 topLeft,
                   glm::vec3 topRight,
                   glm::vec3 bottomLeft,
                   glm::vec3 bottomRight);
};
