#include <iostream>
#include <ostream>
#include <stdexcept>
#include "camera.h"

Camera::Camera(const SceneCameraData& data, int canvasWidth, int canvasHeight, float near, float far)
{
    look = glm::vec3(data.look);
    up = glm::vec3(data.up);
    pos = glm::vec3(data.pos);
    w = -glm::normalize(look);
    v = glm::normalize(up - glm::dot(up, w) * w);
    u = glm::vec4((glm::cross(v, w)), 0);
    glm::mat4 R = glm::mat4(glm::vec4(u.x, v.x, w.x, 0),
                            glm::vec4(u.y, v.y, w.y, 0),
                            glm::vec4(u.z, v.z, w.z, 0),
                            glm::vec4(0, 0, 0, 1));
    glm::mat4 T = glm::mat4(1);
    T[3] = glm::vec4(-pos.x, -pos.y, -pos.z, 1);
    viewMatrix = R * T;
    inverseViewMatrix = glm::inverse(viewMatrix);
    heightAngle = data.heightAngle;
    aspectRatio = canvasWidth / (float) canvasHeight;
    widthAngle = 2 * glm::atan(aspectRatio * glm::tan(heightAngle / 2.0));
    makeProjectionMatrix(near, far);
}

glm::mat4 Camera::getViewMatrix() const {
    // Optional TODO: implement the getter or make your own design
    return viewMatrix;
}

glm::mat4 Camera::getInverseViewMatrix() const
{
    return inverseViewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return projectionMatrix;
}

float Camera::getAspectRatio() const {
    // Optional TODO: implement the getter or make your own design
    return aspectRatio;
}

float Camera::getHeightAngle() const {
    // Optional TODO: implement the getter or make your own design
    return heightAngle;
}

float Camera::getWidthAngle() const
{
    return widthAngle;
}

glm::vec3 Camera::getU() const
{
    return u;
}

glm::vec3 Camera::getV() const
{
    return v;
}

glm::vec3 Camera::getW() const
{
    return w;
}

glm::vec3 Camera::getLook() const
{
    return look;
}

glm::vec3 Camera::getUp() const
{
    return up;
}

glm::vec3 Camera::getPos() const
{
    return pos;
}

// Expects a normalized vector in world space.
void Camera::translate(glm::vec3 worldDir, float amt)
{
    worldDir *= amt;
    pos += worldDir;
    remakeViewMatrix();
}

// Expects a normalized axis in world space.
void Camera::rotate(glm::vec3 axis, float rads)
{
    float cos = glm::cos(rads);
    float sin = glm::sin(rads);
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    glm::mat3 rotation = glm::mat3(glm::vec3(cos + x * x * (1 - cos),
                                             x * y * (1 - cos) + z * sin,
                                             x * z * (1 - cos) - y * sin),
                                   glm::vec3(x * y * (1 - cos) - z * sin,
                                             cos + y * y * (1 - cos),
                                             y * z * (1 - cos) + x * sin),
                                   glm::vec3(x * z * (1 - cos) + y * sin,
                                             y * z * (1 - cos) - x * sin,
                                             cos + z * z * (1 - cos)));
    // We need to update our look and up vectors and then remake the view matrix.
    look = rotation * look;
    up = rotation * up;
    remakeViewMatrix();
}

float Camera::getFocalLength() const {
    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}

float Camera::getAperture() const {
    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}

void Camera::makeProjectionMatrix(float near, float far)
{
    // Projection Matrix stuff
    float c = -near / far;
    float recip_far = 1 / far;
    glm::mat4 scale(glm::vec4(recip_far / glm::tan(widthAngle / 2), 0, 0, 0),
                    glm::vec4(0, recip_far / glm::tan(heightAngle / 2), 0, 0),
                    glm::vec4(0, 0, recip_far, 0),
                    glm::vec4(0, 0, 0, 1));
    glm::mat4 unhinging(glm::vec4(1, 0, 0, 0),
                        glm::vec4(0, 1, 0, 0),
                        glm::vec4(0, 0, 1 / (1 + c), -1),
                        glm::vec4(0, 0, -c / (1 + c), 0));
    glm::mat4 openGL_chicanery(glm::vec4(1, 0, 0, 0),
                               glm::vec4(0, 1, 0, 0),
                               glm::vec4(0, 0, -2, 0),
                               glm::vec4(0, 0, -1, 1));
    projectionMatrix = openGL_chicanery * unhinging * scale;
}

void Camera::remakeViewMatrix()
{
    w = -glm::normalize(look);
    v = glm::normalize(up - glm::dot(up, w) * w);
    u = glm::vec4((glm::cross(v, w)), 0);
    glm::mat4 R = glm::mat4(glm::vec4(u.x, v.x, w.x, 0),
                            glm::vec4(u.y, v.y, w.y, 0),
                            glm::vec4(u.z, v.z, w.z, 0),
                            glm::vec4(0, 0, 0, 1));
    glm::mat4 T = glm::mat4(1);
    T[3] = glm::vec4(-pos.x, -pos.y, -pos.z, 1);
    viewMatrix = R * T;
    inverseViewMatrix = glm::inverse(viewMatrix);
}
