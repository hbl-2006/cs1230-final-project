#version 330 core

layout(location = 0) in vec2 quadOffset;
layout(location = 1) in vec4 instanceData;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float particleSize;

out vec2 uv;
out float lifeFraction;

void main() {
    vec3 center = instanceData.xyz;
    lifeFraction = instanceData.w;

    vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    vec3 cameraUp = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

    vec3 worldPos = center + (cameraRight * quadOffset.x + cameraUp * quadOffset.y) * particleSize;

    uv = quadOffset + 0.5;

    gl_Position = projMatrix * viewMatrix * vec4(worldPos, 1.0);
}
