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
    void updateParticles(float dt);
    void render(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix);
    void finish();

private:
    struct Particle {
        glm::vec3 position;
        float upVelocity;
        float upAcceleration;
        float currLife;
        float maxLife;
    };

    std::vector<Particle> m_particles;
    std::vector<glm::vec4> m_particleData;

    GLuint m_billboardVBO = 0;
    GLuint m_particleVBO = 0;
    GLuint m_VAO = 0;
    GLuint m_shader = 0;

    int m_maxParticles = 200000;
    int m_spawnPerSecond = 1000;
    float m_newParticles = 0.0f;
    float m_maxLifeTime = 1.5f;
    float m_particleSize = 0.1f;
    std::queue<int> m_inactiveParticles;

    void respawnParticle(Particle &p);
};
