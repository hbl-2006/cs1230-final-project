#pragma once

#include "cameraPath.h"

enum class CameraPathTestType {
    YAW_CW,
    YAW_CCW,
    PITCH_UP,
    PITCH_DOWN,
    ROLL_CW,
    ROLL_CCW,
    PATH_CIRCULAR,
    PATH_SPIRAL,
    PATH_ZIGZAG,
    PATH_RANDOM,
    PATH_CIRCULAR_TOWER
};

void useCameraPath(CameraPath& m_cameraPath, CameraPathTestType type);