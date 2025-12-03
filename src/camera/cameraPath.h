#pragma once

#include <glm/glm.hpp>
#include <vector>

struct KeyFrame {
    glm::vec3 pos;

    // in radians
    float yaw;
    float pitch;
    float roll;
};

class CameraPath {
public:
    int frameCount;
    std::vector<KeyFrame> keyframes;
    std::vector<glm::vec3> normals, tangents, c1, c2;
    std::vector<glm::vec3> points;

    CameraPath();
    void addKeyframe(const glm::vec3 &pos, float yaw, float pitch, float roll);
    void buildPath();
    void findSegment(float t, int &segmentIndex, float &currT) const;
    glm::vec3 evaluatePoint(float t) const;
    glm::quat getOrientation(float t) const;

private:
    void computeControls();
};