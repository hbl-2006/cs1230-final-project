#include "particleSystem.h"

#include "../utils/shaderloader.h"
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <vector>

static inline float getRandom() {
    return (rand() % 10000) / 10000.0f;
}

ParticleSystem::ParticleSystem(){}

void ParticleSystem::finish() {
    glDeleteProgram(m_shader);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_billboardVBO);
    glDeleteBuffers(1, &m_particleVBO);
}

void ParticleSystem::initialize() {
    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/particles.vert", ":/resources/shaders/particles.frag");
    
    if (m_maxParticles == 0) {
        while (!m_inactiveParticles.empty()) {
            m_inactiveParticles.pop();
        }
        return;
    }

    m_particles.clear();
    m_particles.resize(m_maxParticles);
    for (int i = 0; i < m_maxParticles; i++) {
        m_particles[i].currLife = -1.0f;
        m_inactiveParticles.push(i);
    }
    m_particleData.clear();
    m_particleData.resize(m_maxParticles);

    std::vector<float> quad = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f,
    };

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_billboardVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_billboardVBO);
    glBufferData(GL_ARRAY_BUFFER, quad.size() * sizeof(float), quad.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void *>(0));

    glGenBuffers(1, &m_particleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_particleVBO);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(GPUParticle), m_particleData.data(), GL_STREAM_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), reinterpret_cast<void *>(0));
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), reinterpret_cast<void *>(offsetof(GPUParticle, lifeFraction)));
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), reinterpret_cast<void *>(offsetof(GPUParticle, particleType)));
    glVertexAttribDivisor(3, 1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::addFireLocation(const glm::vec3 &pos, float scalarImpulse) {
    Key key = {pos.x, pos.z};
    m_fireLocations[key] += scalarImpulse;
}

void ParticleSystem::spawnFireParticles(float dt) {
    float newParticles = dt * m_spawnPerSecond;

    for (auto &[location, impulse] : m_fireLocations) {
        int currNewParticles = (int) (newParticles * sqrt(impulse));
        while (currNewParticles-- > 0) {
            int index = -1;
            if (!m_inactiveParticles.empty()) {
                index = m_inactiveParticles.front();
                m_inactiveParticles.pop();
            } else {
                index = rand() % m_maxParticles;
                if (m_particles[index].currLife > 0.0f) {
                    continue;
                }
            }
            Particle &p = m_particles[index];
            p.position = glm::vec3(location.x + (getRandom() - 0.5f) * 0.125f, -0.5f, location.z + (getRandom() - 0.5f) * sqrt(impulse));
            p.upVelocity = 4.0f + getRandom() * 8.0f;
            p.upAcceleration = getRandom() * 4.0f;
            p.maxLife = m_maxLifeTime * getRandom() * 1.0f;
            p.currLife = p.maxLife;
            p.particleType = 0.0f;
            p.xDir = (getRandom() - 0.5f) * 0.25f;
            p.yDir = getRandom() * 0.25f;
            p.zDir = (getRandom() - 0.5f) * 0.25f;
        }
    }
}

void ParticleSystem::spawnDustParticles(const glm::vec3 &pos, float scalarImpulse) {
    int newParticles = (int) (m_maxDustParticles * scalarImpulse);
    while (newParticles-- > 0) {
        int index = -1;
        if (!m_inactiveParticles.empty()) {
            index = m_inactiveParticles.front();
            m_inactiveParticles.pop();
        } else {
            index = rand() % m_maxParticles;
            if (m_particles[index].currLife > 0.0f) {
                continue;
            }
        }
        Particle &p = m_particles[index];
        p.position = pos + glm::vec3((getRandom() - 0.5f) * 0.5f, 0.0f, (getRandom() - 0.5f) * 0.5f);
        p.upVelocity = (10.0f + getRandom() * 5.0f) * cbrt(scalarImpulse);
        p.maxLife = 5.0f + getRandom() * 0.75f;
        p.currLife = p.maxLife;
        p.upAcceleration = -6.25f * cbrt(scalarImpulse);
        p.particleType = 1.0f;
        p.xDir = (getRandom() - 0.5f) * 0.25f;
        p.yDir = getRandom() * 0.25f;
        p.zDir = (getRandom() - 0.5f) * 0.25f;
    }
}

void ParticleSystem::updateAllParticles(float dt) {
    for (int index = 0; index < m_maxParticles; index++) {
        Particle &p = m_particles[index];
        if (p.currLife > 0) {
            p.currLife -= dt;
            if (p.currLife <= 0) {
                m_inactiveParticles.push(index);
                continue;
            }
            if (p.particleType < 0.5f) {
                float dx = (getRandom() - 0.5f) * 2.5f;
                float dz = (getRandom() - 0.5f) * 2.5f;
                p.position += glm::vec3(dx, p.upVelocity * dt + p.yDir, dz) * 0.1f;
                if (p.currLife / p.maxLife < 0.5f) {
                    p.upVelocity += p.upAcceleration * dt;
                }
            } else {
                float effects = (p.upVelocity > 0.0f) ? 1.0f : 0.75f;
                float dx = p.xDir + (getRandom() - 0.5f) * 0.0625f;
                float dy = p.yDir + (getRandom() - 0.5f) * 0.0625f;
                float dz = p.zDir + (getRandom() - 0.5f) * 0.0625f;
                p.position += glm::vec3(dx, p.upVelocity * dt + dy, dz) * 0.1f * effects;
                p.upVelocity += p.upAcceleration * dt * effects;
            }
        }
    }
}

void ParticleSystem::render(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix) {
    int alive = 0;
    for (auto &p : m_particles) {
        if (p.currLife > 0) {
            float lifeFraction = glm::clamp(p.currLife / p.maxLife, 0.0f, 1.0f);
            m_particleData[alive].position = p.position;
            m_particleData[alive].lifeFraction = lifeFraction;
            m_particleData[alive].particleType = static_cast<float>(p.particleType);
            alive++;
        }
    }
    if (alive == 0) {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_particleVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, alive * sizeof(GPUParticle), m_particleData.data());

    glUseProgram(m_shader);

    glUniformMatrix4fv(glGetUniformLocation(m_shader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "projMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
    glUniform1f(glGetUniformLocation(m_shader, "fireSize"), m_fireSize);
    glUniform1f(glGetUniformLocation(m_shader, "dustSize"), m_dustSize);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glBindVertexArray(m_VAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, alive);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glUseProgram(0);
}
