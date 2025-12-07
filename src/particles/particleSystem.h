#pragma once

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <queue>

class ParticleSystem {
public:
    ParticleSystem();
    void initialize();
    void spawnFireParticles(float dt);
    void updateAllParticles(float dt);
    void spawnDustParticles(const glm::vec3 &pos, float scalarImpulse);
    void render(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix);
    void finish();

private:
    struct Particle {
        glm::vec3 position;
        float upVelocity;
        float upAcceleration;
        float currLife;
        float maxLife;
        int particleType; // 0 for fire, 1 for dust
        float xDir;
        float zDir;
    };

    struct GPUParticle {
        glm::vec3 position;
        float lifeFraction;
        float particleType; // 0 for fire, 1 for dust
    };

    std::vector<Particle> m_particles;
    std::vector<GPUParticle> m_particleData;

    GLuint m_billboardVBO = 0;
    GLuint m_particleVBO = 0;
    GLuint m_VAO = 0;
    GLuint m_shader = 0;

    int m_maxParticles = 2000000;
    float m_spawnPerSecond = 1000.0f;
    float m_maxDustParticles = 50.0f;
    float m_newParticles = 0.0f;
    float m_maxLifeTime = 1.5f;
    float m_fireSize = 0.1f;
    float m_dustSize = 0.75f;
    std::queue<int> m_inactiveParticles;

    void spawnFireParticle(Particle &p);
};
