#version 330 core


layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;


out vec3 worldPos;
out vec3 worldNormal;


uniform mat4 ctm;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normal_mat;

void main() {
    worldPos = vec3(ctm * vec4(position, 1.0));
    worldNormal = normalize(normal_mat * normal);
    gl_Position = proj * view * ctm * vec4(position, 1);
}
