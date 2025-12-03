#include "particleSystem.h"

#include "../utils/shaderloader.h"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

static inline float getRandom() {
    return (arc4random() % 10000) / 10000.0f;
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

    m_particles.clear();
    m_particles.resize(m_maxParticles);
    for (int i = 0; i < m_maxParticles; i++) {
        m_particles[i].currLife = -1.0f;
        m_inactiveParticles.push(i);
    }
    m_particleData.clear();
    m_particleData.resize(m_maxParticles, glm::vec4(0.0f));
    if (m_maxParticles == 0) {
        while (!m_inactiveParticles.empty()) {
            m_inactiveParticles.pop();
        }
        return;
    }

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
    glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(glm::vec4), m_particleData.data(), GL_STREAM_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), reinterpret_cast<void *>(0));

    glVertexAttribDivisor(1, 1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::respawnParticle(Particle &p) {
    p.position = glm::vec3(4.5f + (getRandom() - 0.5f) * 0.25f, 1.5f, 4.5f + (getRandom() - 0.5f) * 0.25f);
    p.upVelocity = 2.0f + getRandom() * 4.0f;
    p.upAcceleration = getRandom() * 4.0f;
    p.maxLife = m_maxLifeTime * getRandom();
    p.currLife = p.maxLife;
}

void ParticleSystem::updateParticles(float dt) {
    if (m_maxParticles == 0) {
        return;
    }
    m_newParticles += dt * m_spawnPerSecond;

    while (m_newParticles >= 1.0f) {
        if (!m_inactiveParticles.empty()) {
            int index = m_inactiveParticles.front();
            m_inactiveParticles.pop();
            respawnParticle(m_particles[index]);
        } else {
            respawnParticle(m_particles[arc4random() % m_maxParticles]);
        }
        m_newParticles -= 1.0f;
    }

    for (int index = 0; index < m_maxParticles; index++) {
        Particle &p = m_particles[index];
        if (p.currLife > 0) {
            p.currLife -= dt;
            if (p.currLife <= 0) {
                m_inactiveParticles.push(index);
                continue;
            }
            float dx = (getRandom() - 0.5f) * 0.125f;
            float dz = (getRandom() - 0.5f) * 0.125f;
            p.position += glm::vec3(dx, p.upVelocity * dt, dz) * 0.1f;
            if (p.currLife / p.maxLife < 0.5f) {
                p.upVelocity += p.upAcceleration * dt;
            }
        }
    }
}

void ParticleSystem::render(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix) {
    int alive = 0;
    for (auto &p : m_particles) {
        if (p.currLife > 0) {
            float lifeFraction = glm::clamp(p.currLife / p.maxLife, 0.0f, 1.0f);
            m_particleData[alive++] = glm::vec4(p.position, lifeFraction);
        }
    }
    if (alive == 0) {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_particleVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, alive * sizeof(glm::vec4), m_particleData.data());

    glUseProgram(m_shader);

    glUniformMatrix4fv(glGetUniformLocation(m_shader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "projMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
    glUniform1f(glGetUniformLocation(m_shader, "particleSize"), m_particleSize);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    glBindVertexArray(m_VAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, alive);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glUseProgram(0);
}
