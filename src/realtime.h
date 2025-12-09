#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#include "camera/camera.h"
#include "shapes/Cone.h"
#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Sphere.h"
#include "utils/sceneparser.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>
#include "camera/cameraPath.h"
#include "particles/particleSystem.h"

class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish(); // Called on program exit
    void sceneChanged();
    void settingsChanged();
    void saveViewportImage(std::string filePath);

public slots:
    void tick(QTimerEvent *event); // Called once per tick of m_timer

protected:
    void initializeGL() override; // Called once at the start of the program
    void paintGL() override; // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override; // Called when window size changes

private:
    bool initialized = false;

    // Scenefile Stuff
    RenderData metadata;
    Camera camera;
    CameraPath cameraPath;
    bool prevP;
    bool prevO;
    bool prevG;
    bool prevX;

    // enum for each shape to make accessing stuff easier.
    enum ShapeNames { SPHERE, CONE, CUBE, CYLINDER };
    // one copy of the unit sphere, cone, cube, and cylinder. every object has the same vertices, so we can use these for each object in the scene.
    Sphere sphere;
    Cone cone;
    Cube cube;
    Cylinder cylinder;
    void initializeData(ShapeNames name);
    void drawShape(RenderShapeData &shape,
                   GLint useTextureLoc,
                   GLint textureSamplerLoc,
                   GLint blendLoc,
                   GLint repeatULoc,
                   GLint repeatVLoc,
                   GLint useNormalMapLoc,
                   GLint normalSamplerLoc,
                   GLint useBumpMapLoc,
                   GLint bumpSamplerLoc,
                   GLint bumpStrengthLoc,
                   GLint useParallaxMapLoc,
                   GLint parallaxSamplerLoc,
                   GLint parallaxHeightLoc);

    // Texture loading and caching
    std::unordered_map<std::string, GLuint> m_textureCache;
    GLuint loadTexture(const std::string &filepath);

    // Cached vertex counts for efficient rendering
    int sphereVertexCount = 0;
    int coneVertexCount = 0;
    int cubeVertexCount = 0;
    int cylinderVertexCount = 0;

    // Copies of pertinent settings to check if changed
    int param1;
    int param2;
    float near;
    float far;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    // Tick Related Variables
    int m_timer;                  // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer; // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                   // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                 // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap; // Stores whether keys are pressed or not

    // Device Correction Variables
    double m_devicePixelRatio;

    // vbo indices
    uint vbos[4];

    // vao indices
    uint vaos[4];

    // shader
    uint m_shader;

    ParticleSystem particles;

    // Physics
    bool forces = false;
    std::vector<RigidBody *> sortedBodies;
    void stepPhysics(float deltaTime);
    void resolveCollisions();
    void updateBoundingBoxes();
    void sweepAndPrune();
    void resolveOneCollision(RigidBody *A, RigidBody *B);
    glm::vec3 calculateMTV(RigidBody *A, RigidBody *B);
    glm::vec3 approximateContactPoint(RigidBody *A, RigidBody *B);

    // particles
    bool dustOn = false;
};
