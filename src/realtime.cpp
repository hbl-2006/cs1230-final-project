#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include "utils/sceneparser.h"
#include "utils/shaderloader.h"
#include <glm/gtx/string_cast.hpp>
#include "camera/cameraPath.h"
#include "camera/cameraPathTest.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

// ================== Rendering the Scene!

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
    , camera(metadata.cameraData,
             size().width(),
             size().height(),
             settings.nearPlane,
             settings.farPlane)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;
    m_keyMap[Qt::Key_P]       = false;
    prevP                     = false;
    m_keyMap[Qt::Key_O]       = false;
    prevO                     = false;
    m_keyMap[Qt::Key_G]       = false;
    prevG                     = false;
    m_keyMap[Qt::Key_X]       = false;
    prevX                     = false;

    // If you must use this function, do not edit anything above this
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here
    glDeleteBuffers(4, vbos);
    glDeleteBuffers(4, vaos);
    glDeleteProgram(m_shader);
    this->doneCurrent();

    particles.finish();
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert",
                                                 ":/resources/shaders/default.frag");

    // make all the vbos and vaos
    glGenBuffers(4, vbos);
    glGenVertexArrays(4, vaos);

    // initialize all vbos and vaos containing unit vertex data that will be eventually passed to shaders
    initializeData(sphere, SPHERE);
    initializeData(cone, CONE);
    initializeData(cube, CUBE);
    initializeData(cylinder, CYLINDER);
    
    cameraPath = CameraPath();
    useCameraPath(cameraPath, CameraPathTestType::PATH_CIRCULAR_TOWER); // pick path here for camera path
    cameraPath.buildPath();

    srand(time(NULL));
    particles.initialize();

    initialized = true;
}

void Realtime::paintGL() {
    // Students: anything requiring OpenGL calls every frame should be done here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glUseProgram(m_shader);
    // draw each shape in our render data
    auto worldCam = camera.getInverseViewMatrix() * glm::vec4(0, 0, 0, 1);
    glUniform3f(glGetUniformLocation(m_shader, "camPos"), worldCam.x, worldCam.y, worldCam.z);
    for (auto &shape : metadata.shapes) {
        //std::cout << "painting a shape" << std::endl;
        drawShape(shape);
    }
    glUseProgram(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    particles.render(camera.getViewMatrix(), camera.getProjectionMatrix());
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
}

void Realtime::sceneChanged() {
    // update our metadata with the new scenefile
    bool success = SceneParser::parse(settings.sceneFilePath, metadata);
    prevP = false;
    prevO = false;
    prevG = false;
    prevX = false;
    dustOn = false;

    // update our list of sortedBodies
    sortedBodies.clear();
    for (auto &shape : metadata.shapes) {
        sortedBodies.push_back(&(shape.body));
    }
    //std::cout << "bodies updated" << std::endl;
    camera = Camera(metadata.cameraData,
                    size().width(),
                    size().height(),
                    settings.nearPlane,
                    settings.farPlane);
  
    camera = Camera(metadata.cameraData,
                    size().width(),
                    size().height(),
                    settings.nearPlane,
                    settings.farPlane);
    camera.setPathTime(0.0f);

    // Populate our shader uniforms
    glUseProgram(m_shader);
    glUniform1f(glGetUniformLocation(m_shader, "ambient"), metadata.globalData.ka);
    glUniform1f(glGetUniformLocation(m_shader, "diffuse"), metadata.globalData.kd);
    glUniform1f(glGetUniformLocation(m_shader, "specular"), metadata.globalData.ks);
    auto worldCam = camera.getInverseViewMatrix() * glm::vec4(0, 0, 0, 1);
    glUniform3f(glGetUniformLocation(m_shader, "camPos"), worldCam.x, worldCam.y, worldCam.z);

    glUniform1i(glGetUniformLocation(m_shader, "numLights"), (GLint) metadata.lights.size());
    for (int i = 0; i < metadata.lights.size(); i++) {
        auto light = metadata.lights[i];
        GLint posLoc = glGetUniformLocation(m_shader,
                                            ("lights[" + std::to_string(i) + "].pos").c_str());
        glm::vec3 pos = light.pos;
        glUniform3f(posLoc, pos.x, pos.y, pos.z);

        GLint typeLoc = glGetUniformLocation(m_shader,
                                             ("lights[" + std::to_string(i) + "].type").c_str());
        int type = -1;
        switch (light.type) {
        case LightType::LIGHT_POINT:
            type = 0;
            break;
        case LightType::LIGHT_DIRECTIONAL:
            type = 1;
            break;
        case LightType::LIGHT_SPOT:
            type = 2;
            break;
        }
        glUniform1i(typeLoc, type);

        GLint colLoc = glGetUniformLocation(m_shader,
                                            ("lights[" + std::to_string(i) + "].col").c_str());
        glm::vec3 col = light.color;
        glUniform3f(colLoc, col.x, col.y, col.z);

        GLint attLoc = glGetUniformLocation(m_shader,
                                            ("lights[" + std::to_string(i) + "].att").c_str());
        glm::vec3 att = light.function;
        glUniform3f(attLoc, att.x, att.y, att.z);

        GLint dirLoc = glGetUniformLocation(m_shader,
                                            ("lights[" + std::to_string(i) + "].dir").c_str());
        glm::vec3 dir = light.dir;
        glUniform3f(dirLoc, dir.x, dir.y, dir.z);

        GLint angleLoc = glGetUniformLocation(m_shader,
                                              ("lights[" + std::to_string(i) + "].angle").c_str());
        glUniform1f(angleLoc, light.angle);

        GLint penumbraLoc
            = glGetUniformLocation(m_shader, ("lights[" + std::to_string(i) + "].penumbra").c_str());
        glUniform1f(penumbraLoc, light.penumbra);
    }
    glUseProgram(0);

    if (!success) {
        std::cerr << "Error loading scene: \"" << settings.sceneFilePath << "\"" << std::endl;
        exit(1);
    }
    //metadata.shapes[0].body.addImpulse(glm::vec3(1, 0, 0));
    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {
    // only do this stuff if opengl is actually initialized
    if (initialized) {
        // reinitialize vbos/vaos if our shape params changed
        if (settings.shapeParameter1 != param1 || settings.shapeParameter2 != param2) {
            initializeData(sphere, SPHERE);
            initializeData(cone, CONE);
            initializeData(cube, CUBE);
            initializeData(cylinder, CYLINDER);
        }
        // remake projection matrix if near/far plane changed
        if (settings.nearPlane != near || settings.farPlane != far) {
            camera.makeProjectionMatrix(settings.nearPlane, settings.farPlane);
        }
    }
    param1 = settings.shapeParameter1;
    param2 = settings.shapeParameter2;
    near = settings.nearPlane;
    far = settings.farPlane;
    update(); // asks for a PaintGL() call to occur
}

// ================== Camera Movement!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate
        float xProportion = (float) deltaX / size().width();
        float yProportion = (float) deltaY / size().height();
        float fullSend = M_PI;
        float xRads = xProportion * fullSend;
        float yRads = yProportion * fullSend;
        camera.rotate(glm::vec3(0, 1, 0), xRads);
        camera.rotate(glm::normalize(glm::cross(camera.getLook(), camera.getUp())), yRads);
        update(); // asks for a PaintGL() call to occur
    }
}

glm::vec3 extractYawPitchRoll(glm::vec3 look, glm::vec3 up) {
    glm::vec3 f = glm::normalize(look);
    glm::vec3 r = glm::normalize(glm::cross(f, up));
    glm::vec3 u = glm::normalize(glm::cross(r, f));

    glm::mat3 R(
        r.x, u.x, -f.x,
        r.y, u.y, -f.y,
        r.z, u.z, -f.z
    );

    float yaw = std::atan2(R[0][2], R[2][2]);
    float pitch = std::asin(-R[1][2]);
    float roll = std::atan2(R[1][0], R[1][1]);

    return glm::vec3(yaw, pitch, roll);
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around
    if (m_keyMap[Qt::Key_P] && !prevP) {
        camera.toggleCameraPath();
        if (camera.useCameraPathEnabled()) {
            camera.setPathTime(0.0f);

            glm::vec3 newPos = cameraPath.evaluatePoint(0.0f);
            glm::quat rotation = cameraPath.getOrientation(0.0f);
            glm::vec4 newLook = glm::normalize(rotation * glm::vec4(0, 0, -1, 0));
            glm::vec4 newUp = glm::normalize(rotation * glm::vec4(0, 1, 0, 0));

            camera.setPosition(glm::vec4(newPos, 1.0f));
            camera.setLookandUp(newLook, newUp);
        }
    }
    prevP = m_keyMap[Qt::Key_P];

    if (m_keyMap[Qt::Key_O] && !prevO) {
        dustOn = !dustOn;
        metadata.shapes[249].body.addImpulse(glm::vec3(1, 0, 1));
        metadata.shapes[249].body.addAngularImpulse(glm::vec3(0, 0, -1));
        metadata.shapes[250].body.addImpulse(glm::vec3(-1, 0, -1));
        metadata.shapes[250].body.addAngularImpulse(glm::vec3(0, 0, 1));
    }
    prevO = m_keyMap[Qt::Key_O];

    if (m_keyMap[Qt::Key_G] && !prevG) {
        forces = !forces;
    }
    prevG = m_keyMap[Qt::Key_G];

    if (m_keyMap[Qt::Key_X] && !prevX) {
        glm::vec3 cameraPos = glm::vec3(camera.getPos());
        glm::vec3 look = glm::vec3(camera.getLook());
        glm::vec3 up = glm::vec3(camera.getUp());
        std::cout << "Camera Position: " << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << std::endl;

        glm::vec3 ypr = extractYawPitchRoll(look, up);
        std::cout << "Yaw:   " << ypr.x << " rad (" << glm::degrees(ypr.x) << " deg)" << std::endl;
        std::cout << "Pitch: " << ypr.y << " rad (" << glm::degrees(ypr.y) << " deg)" << std::endl;
        std::cout << "Roll:  " << ypr.z << " rad (" << glm::degrees(ypr.z) << " deg)" << std::endl;
    }
    prevX = m_keyMap[Qt::Key_X];

    if (!camera.useCameraPathEnabled()) {
        auto worldSpaceVec = glm::vec3(0);
        if (m_keyMap[Qt::Key_Space]) {
            worldSpaceVec += glm::vec3(0, 1, 0);
        }
        if (m_keyMap[Qt::Key_Control]) {
            worldSpaceVec += glm::vec3(0, -1, 0);
        }
        if (m_keyMap[Qt::Key_W]) {
            worldSpaceVec += glm::normalize(camera.getLook());
        }
        if (m_keyMap[Qt::Key_S]) {
            worldSpaceVec += -glm::normalize(camera.getLook());
        }
        if (m_keyMap[Qt::Key_A]) {
            worldSpaceVec += -glm::normalize(glm::cross(camera.getLook(), camera.getUp()));
        }
        if (m_keyMap[Qt::Key_D]) {
            worldSpaceVec += glm::normalize(glm::cross(camera.getLook(), camera.getUp()));
        }
        if (worldSpaceVec != glm::vec3(0)) {
            worldSpaceVec = glm::normalize(worldSpaceVec);
        }
        camera.translate(worldSpaceVec, deltaTime * 5);
    } else {
        camera.updatePathTime(deltaTime * 0.5f);
        glm::vec3 newPos = cameraPath.evaluatePoint(camera.getPathTime());
        glm::quat rotation = cameraPath.getOrientation(camera.getPathTime());
        glm::vec4 newLook = glm::normalize(rotation * glm::vec4(0, 0, -1, 0));
        glm::vec4 newUp = glm::normalize(rotation * glm::vec4(0, 1, 0, 0));

        camera.setPosition(glm::vec4(newPos, 1.0f));
        camera.setLookandUp(newLook, newUp);
    }

    particles.spawnFireParticles(deltaTime);
    particles.updateAllParticles(deltaTime);

    // Physics updates: before we draw! Additionally, update the ctm before drawing.
    if (metadata.shapes.size() > 0) {
        float substep = 0.005f; // split physics ticks into 5ms set timesteps.
        float timeLeft = deltaTime;

        while (timeLeft > 0) {
            float dt = std::min(substep, timeLeft);
            stepPhysics(dt);
            resolveCollisions();
            timeLeft -= dt;
        }
    }
    update(); // asks for a PaintGL() call to occur
}

// DO NOT EDIT
void Realtime::saveViewportImage(std::string filePath) {
    // Make sure we have the right context and everything has been drawn
    makeCurrent();

    int fixedWidth = 1024;
    int fixedHeight = 768;

    // Create Frame Buffer
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create a color attachment texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fixedWidth, fixedHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Optional: Create a depth buffer if your rendering uses depth testing
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fixedWidth, fixedHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    // Render to the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, fixedWidth, fixedHeight);

    // Clear and render your scene here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintGL();

    // Read pixels from framebuffer
    std::vector<unsigned char> pixels(fixedWidth * fixedHeight * 3);
    glReadPixels(0, 0, fixedWidth, fixedHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Unbind the framebuffer to return to default rendering to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Convert to QImage
    QImage image(pixels.data(), fixedWidth, fixedHeight, QImage::Format_RGB888);
    QImage flippedImage = image.mirrored(); // Flip the image vertically

    // Save to file using Qt
    QString qFilePath = QString::fromStdString(filePath);
    if (!flippedImage.save(qFilePath)) {
        std::cerr << "Failed to save image to " << filePath << std::endl;
    }

    // Clean up
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}
