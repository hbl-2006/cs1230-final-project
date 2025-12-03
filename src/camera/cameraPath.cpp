#include "cameraPath.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

CameraPath::CameraPath() {
    frameCount = 0;
    keyframes.clear();
    normals.clear();
    tangents.clear();
    c1.clear();
    c2.clear();
}

void CameraPath::addKeyframe(const glm::vec3 &pos, float yaw, float pitch, float roll) {
    KeyFrame frame;
    frame.pos = pos;
    frame.yaw = yaw;
    frame.pitch = pitch;
    frame.roll = roll;
    keyframes.push_back(frame);
    frameCount++;
}

void CameraPath::buildPath() {
    if(frameCount < 2) {
        return;
    }

    computeControls();
}

glm::vec3 interpolate(const glm::vec3& p1, const glm::vec3& p2, float u) {
    return p1 + u * (p2 - p1);
}

void CameraPath::computeControls(){
    normals.resize(frameCount);
    tangents.resize(frameCount);
    c1.resize(frameCount);
    c2.resize(frameCount);

    for(int i = 1; i < frameCount - 1; i++) {
        normals[i] = interpolate(keyframes[i - 1].pos, keyframes[i].pos, 2.0f);
    }

    for(int i = 1; i < frameCount - 1; i++) {
        tangents[i] = interpolate(normals[i], keyframes[i + 1].pos, 0.5f);
    }

    for(int i = 1; i < frameCount - 1; i++) {
        c1[i] = interpolate(keyframes[i].pos, tangents[i], 1.0f / 3.0f);
    }
    
    for(int i = 0; i < frameCount - 2; i++) {
        c2[i] = interpolate(keyframes[i + 1].pos, c1[i + 1], -1.0f);
    }

    c1[0] = interpolate(keyframes[0].pos, interpolate(keyframes[1].pos, c2[0], 3.0f / 2.0f), 2.0f / 3.0f);
    c2[frameCount - 2] = interpolate(keyframes[frameCount - 1].pos, interpolate(keyframes[frameCount - 2].pos, c1[frameCount - 2], 3.0f / 2.0f), 2.0f / 3.0f);
}

void CameraPath::findSegment(float t, int &segmentIndex, float &currT) const {
    t = glm::clamp(t, 0.0f, 1.0f);
    float segment = t * (frameCount - 1);
    segmentIndex = glm::clamp((int) (glm::floor(segment)), 0, frameCount - 2);
    currT = segment - (float) segmentIndex;
}

glm::vec3 calculatePoint(glm::vec3 Ki, glm::vec3 C1, glm::vec3 C2, glm::vec3 Kii, float t) {
    std::vector<glm::vec3> points = {Ki, C1, C2, Kii};
    for(int i = 3; i > 0; i--) {
        for(int j = 0; j < i; j++) {
            points[j] = interpolate(points[j], points[j + 1], t);
        }
    }
    return points[0];
}

glm::vec3 CameraPath::evaluatePoint(float t) const {
    if(frameCount < 2) {
        return frameCount == 1 ? keyframes[0].pos : glm::vec3(0.0f);
    }
    int segmentIndex;
    float currT;
    findSegment(t, segmentIndex, currT);

    glm::vec3 Ki = keyframes[segmentIndex].pos;
    glm::vec3 C1 = c1[segmentIndex];
    glm::vec3 C2 = c2[segmentIndex];
    glm::vec3 Kii = keyframes[segmentIndex + 1].pos;

    return calculatePoint(Ki, C1, C2, Kii, currT);
}

glm::quat CameraPath::getOrientation(float t) const {
    if (keyframes.size() < 2)
        return glm::quat(1, 0, 0, 0);

    int segmentIndex;
    float currT;
    findSegment(t, segmentIndex, currT);

    const KeyFrame& k0 = keyframes[segmentIndex];
    const KeyFrame& k1 = keyframes[segmentIndex + 1];

    float yaw = k0.yaw + currT * (k1.yaw - k0.yaw);
    float pitch = k0.pitch + currT * (k1.pitch - k0.pitch);
    float roll = k0.roll + currT * (k1.roll - k0.roll);

    return glm::yawPitchRoll(yaw, pitch, roll);
}
