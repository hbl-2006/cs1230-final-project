#version 330 core

layout(location = 0) in vec2 quadOffset;
layout(location = 1) in vec3 position;
layout(location = 2) in float lifeFractionIn;
layout(location = 3) in float particleTypeIn;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float fireSize;
uniform float dustSize;

out vec2 uv;
out float lifeFraction;
out float particleType;

void main() {
    vec3 center = position;
    lifeFraction = lifeFractionIn;
    particleType = particleTypeIn;

    vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    vec3 cameraUp = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

    float size = -1.0;
    if (particleType < 0.5) {
        size = mix(fireSize * 1.0, fireSize * 0.25, 1.0 - lifeFraction);
    } else {
        size = mix(dustSize * 0.5, dustSize * 7.5, 1.0 - lifeFraction);
    }
    vec3 worldPos = center + (cameraRight * quadOffset.x + cameraUp * quadOffset.y) * size;

    uv = quadOffset + 0.5;

    gl_Position = projMatrix * viewMatrix * vec4(worldPos, 1.0);
}
