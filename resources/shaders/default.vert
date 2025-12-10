#version 330 core

// Input attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

// Output to fragment shader
out vec3 worldPos;
out vec3 worldNormal;
out vec2 texCoords;
out mat3 TBN;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    // Transform position to world space
    worldPos = vec3(model * vec4(position, 1.0));

    // Calculate normal matrix for proper normal transformation
    mat3 normalMatrix = mat3(transpose(inverse(model)));

    // Transform normal to world space
    worldNormal = normalize(normalMatrix * normal);

    // Transform tangent and bitangent to world space for normal/bump mapping
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = worldNormal;

    // Create TBN matrix for tangent space calculations
    TBN = mat3(T, B, N);

    // Pass through texture coordinates
    texCoords = uv;

    // Final position in clip space
    gl_Position = proj * view * model * vec4(position, 1.0);
}
