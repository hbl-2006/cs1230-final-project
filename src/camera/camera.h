#pragma once

#include "utils/scenedata.h"
#include <glm/glm.hpp>

// A class representing a virtual camera.

// Feel free to make your own design choices for Camera class, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Camera {
public:
    Camera(const SceneCameraData& data, int canvasWidth, int canvasHeight, float near, float far);

    // Returns the view matrix for the current camera settings.
    // You might also want to define another function that return the inverse of the view matrix.
    glm::mat4 getViewMatrix() const;

    glm::mat4 getInverseViewMatrix() const;

    void makeProjectionMatrix(float near, float far);

    glm::mat4 getProjectionMatrix() const;

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

    // Returns the width angle of the camera in RADIANS.
    float getWidthAngle() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getFocalLength() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getAperture() const;

    glm::vec3 getU() const;
    glm::vec3 getV() const;
    glm::vec3 getW() const;
    glm::vec3 getLook() const;
    glm::vec3 getUp() const;
    glm::vec3 getPos() const;

    void translate(glm::vec3 along, float amt);
    void rotate(glm::vec3 axis, float rads);

private:
    glm::mat4 viewMatrix;
    glm::mat4 inverseViewMatrix;
    glm::mat4 projectionMatrix;
    float aspectRatio;
    float heightAngle;
    float widthAngle;
    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 w;
    glm::vec3 look;
    glm::vec3 up;
    glm::vec3 pos;
    void remakeViewMatrix();
};
