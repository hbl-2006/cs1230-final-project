#pragma once

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <queue>
#include <unordered_map>

class ParticleSystem {
public:
    ParticleSystem();
    void initialize();
    void spawnFireParticles(float dt);
    void updateAllParticles(float dt);
    void spawnDustParticles(const glm::vec3 &pos, float scalarImpulse);
    void render(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix);
    void finish();
    void addFireLocation(const glm::vec3 &pos, float scalarImpulse);

    struct Key {
        float x, z;
        bool operator==(const Key &other) const {
            return x == other.x && z == other.z;
        }
    };

private:
    struct Particle {
        glm::vec3 position;
        float upVelocity;
        float upAcceleration;
        float currLife;
        float maxLife;
        int particleType; // 0 for fire, 1 for dust
        float xDir;
        float yDir;
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

    int m_maxParticles = 10000000;
    float m_spawnPerSecond = 5000.0f;
    float m_maxLifeTime = 1.5f;
    float m_fireSize = 0.5f;
    float m_maxDustParticles = 50.0f;
    float m_dustSize = 0.75f;
    std::queue<int> m_inactiveParticles;

    struct KeyHash {
        size_t operator()(const Key &k) const {
            size_t hx = std::hash<float>{}(k.x);
            size_t hz = std::hash<float>{}(k.z);
            return hx ^ (hz + 0x9e3779b97f4a7c15ULL + (hx << 6) + (hx >> 2));
        }
    };
    std::unordered_map<Key, float, KeyHash> m_fireLocations;
};
