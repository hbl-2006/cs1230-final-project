#include "cameraPathTest.h"
#include "cameraPath.h"

void useCameraPath(CameraPath &m_cameraPath, CameraPathTestType type)
{
    switch (type)
    {
    case CameraPathTestType::YAW_CW:
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(405.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(315.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(225.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(135.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(0.0f));
        break;
    case CameraPathTestType::YAW_CCW:
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(135.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(225.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(315.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(405.0f), glm::radians(-45.0f), glm::radians(0.0f));
        break;
    case CameraPathTestType::PITCH_UP:
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-405.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-315.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-225.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-135.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(0.0f));
        break;
    case CameraPathTestType::PITCH_DOWN:
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-135.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-225.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-315.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-405.0f), glm::radians(0.0f));
        break;
    case CameraPathTestType::ROLL_CW:
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(360.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(270.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(180.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(90.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(0.0f));
        break;
    case CameraPathTestType::ROLL_CCW:
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(90.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(180.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(270.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 4.0f, 4.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(360.0f));
        break;
    case CameraPathTestType::PATH_CIRCULAR:
        m_cameraPath.addKeyframe(glm::vec3(10.0f, 6.0f, 10.0f), glm::radians(45.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f + 6.0f * sqrt(2), 6.0f, 4.0f), glm::radians(90.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(10.0f, 6.0f, -2.0f), glm::radians(135.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 6.0f, 4.0f - 6.0f * sqrt(2)), glm::radians(180.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(-2.0f, 6.0f, -2.0f), glm::radians(225.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f - 6.0f * sqrt(2), 6.0f, 4.0f), glm::radians(270.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(-2.0f, 6.0f, 10.0f), glm::radians(315.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 6.0f, 4.0f + 6.0f * sqrt(2)), glm::radians(360.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(10.0f, 6.0f, 10.0f), glm::radians(405.0f), glm::radians(-45.0f), glm::radians(0.0f));
        break;
    case CameraPathTestType::PATH_SPIRAL:
        m_cameraPath.addKeyframe(glm::vec3(32.0f, 16.0f, 32.0f), glm::radians(45.0f), glm::radians(0.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(28.0f, 14.0f, 28.0f), glm::radians(45.0f), glm::radians(0.0f), glm::radians(180.0f));
        m_cameraPath.addKeyframe(glm::vec3(24.0f, 12.0f, 24.0f), glm::radians(45.0f), glm::radians(0.0f), glm::radians(360.0f));
        m_cameraPath.addKeyframe(glm::vec3(20.0f, 10.0f, 20.0f), glm::radians(45.0f), glm::radians(0.0f), glm::radians(540.0f));
        m_cameraPath.addKeyframe(glm::vec3(16.0f, 8.0f, 16.0f), glm::radians(45.0f), glm::radians(0.0f), glm::radians(720.0f));
        m_cameraPath.addKeyframe(glm::vec3(12.0f, 6.0f, 12.0f), glm::radians(45.0f), glm::radians(0.0f), glm::radians(900.0f));
        m_cameraPath.addKeyframe(glm::vec3(8.0f, 4.0f, 8.0f), glm::radians(45.0f), glm::radians(0.0f), glm::radians(1080.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 2.0f, 4.0f), glm::radians(45.0f), glm::radians(0.0f), glm::radians(1260.0f));
        m_cameraPath.addKeyframe(glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(45.0f), glm::radians(0.0f), glm::radians(1440.0f));
        break;
    case CameraPathTestType::PATH_ZIGZAG:
        m_cameraPath.addKeyframe(glm::vec3(32.0f, 24.0f, 0.0f), glm::radians(135.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(0.0f, 21.0f, 28.0f), glm::radians(-45.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(24.0f, 18.0f, 0.0f), glm::radians(135.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(0.0f, 15.0f, 20.0f), glm::radians(-45.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(16.0f, 12.0f, 0.0f), glm::radians(135.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(0.0f, 9.0f, 12.0f), glm::radians(-45.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(12.0f, 6.0f, 0.0f), glm::radians(135.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(0.0f, 3.0f, 4.0f), glm::radians(-45.0f), glm::radians(-45.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(135.0f), glm::radians(-45.0f), glm::radians(0.0f));
        break;
    case CameraPathTestType::PATH_RANDOM:
        m_cameraPath.addKeyframe(glm::vec3(0.0f, 4.0f, 6.0f), glm::radians(0.0f), glm::radians(-15.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(4.0f, 2.5f, 7.0f), glm::radians(90.0f), glm::radians(-15.0f), glm::radians(30.0f));
        m_cameraPath.addKeyframe(glm::vec3(7.0f, 6.0f, 8.0f), glm::radians(0.0f), glm::radians(-45.0f), glm::radians(60.0f));
        m_cameraPath.addKeyframe(glm::vec3(5.0f, 8.0f, 10.0f), glm::radians(-90.0f), glm::radians(-15.0f), glm::radians(280.0f));
        m_cameraPath.addKeyframe(glm::vec3(1.0f, 12.0f, 8.0f), glm::radians(-180.0f), glm::radians(-90.0f), glm::radians(90.0f));
        m_cameraPath.addKeyframe(glm::vec3(0.0f, 8.0f, 7.0f), glm::radians(-360.0f), glm::radians(-90.0f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(0.0f, 4.0f, 6.0f), glm::radians(-540.0f), glm::radians(-45.0f), glm::radians(-90.0f));
        m_cameraPath.addKeyframe(glm::vec3(0.0f, 4.0f, 6.0f), glm::radians(-360.0f), glm::radians(-15.0f), glm::radians(0.0f));
        break;
    case CameraPathTestType::PATH_CIRCULAR_TOWER:
        m_cameraPath.addKeyframe(glm::vec3(0.0f,                        9.0f,   18.0f),
                                glm::radians(0.0f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(9.0 * sqrt(2 - sqrt(2)),     9.0f,   9.0 * sqrt(2 + sqrt(2))),
                                glm::radians(22.5f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(9.0 * sqrt(2),               9.0f,   9.0 * sqrt(2)),
                                glm::radians(45.0f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(9.0 * sqrt(2 + sqrt(2)),     9.0f,   9.0 * sqrt(2 - sqrt(2))),
                                glm::radians(67.5f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(18.0f,                       9.0f,   0.0f),
                                glm::radians(90.0f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(9.0 * sqrt(2 + sqrt(2)),     9.0f,   -9.0 * sqrt(2 - sqrt(2))),
                                glm::radians(112.5f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(9.0 * sqrt(2),               9.0f,   -9.0 * sqrt(2)),
                                glm::radians(135.0f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(9.0 * sqrt(2 - sqrt(2)),     9.0f,   -9.0 * sqrt(2 + sqrt(2))),
                                glm::radians(157.5f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(0.0f,                        9.0f,   -18.0f),
                                glm::radians(180.0f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(-9.0 * sqrt(2 - sqrt(2)),    9.0f,   -9.0 * sqrt(2 + sqrt(2))),
                                glm::radians(202.5f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(-9.0 * sqrt(2),              9.0f,   -9.0 * sqrt(2)),
                                glm::radians(225.0f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(-9.0 * sqrt(2 + sqrt(2)),    9.0f,   -9.0 * sqrt(2 - sqrt(2))),
                                glm::radians(247.5f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(-18.0f,                      9.0f,   0.0f),
                                glm::radians(270.0f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(-9.0 * sqrt(2 + sqrt(2)),    9.0f,   9.0 * sqrt(2 - sqrt(2))),
                                glm::radians(292.5f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(-9.0 * sqrt(2),              9.0f,   9.0 * sqrt(2)),
                                glm::radians(315.0f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(-9.0 * sqrt(2 - sqrt(2)),    9.0f,   9.0 * sqrt(2 + sqrt(2))),
                                glm::radians(337.5f), glm::radians(-22.5f), glm::radians(0.0f));
        m_cameraPath.addKeyframe(glm::vec3(0.0f,                        9.0f,   18.0f),
                                glm::radians(360.0f), glm::radians(-22.5f), glm::radians(0.0f));
    }
}