#version 330 core

// Input from vertex shader
in vec3 worldPos;
in vec3 worldNormal;
in vec2 texCoords;
in mat3 TBN;

// Output
out vec4 fragColor;

struct Light {
    int type;        // 0 = point, 1 = directional, 2 = spot
    vec3 pos;
    vec3 col;
    vec3 dir;
    vec3 att;
    float angle;
    float penumbra;
};

uniform Light lights[8];
uniform int numLights;
uniform float ambient;
uniform float diffuse;
uniform float specular;
uniform vec3 camPos;

uniform vec4 cAmbient;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform float shininess;

uniform int useTexture;
uniform sampler2D textureSampler;
uniform float blend;
uniform float repeatU;
uniform float repeatV;

uniform int useNormalMap;
uniform sampler2D normalSampler;

uniform int useBumpMap;
uniform sampler2D bumpSampler;
uniform float bumpStrength;

uniform int useParallaxMap;
uniform sampler2D parallaxSampler;
uniform float parallaxHeight;

// Spot light falloff function
float spotLightFalloff(float x, float inner, float outer) {
    return -2.0 * pow(((x - inner) / (outer - inner)), 3.0)
           + 3.0 * pow(((x - inner) / (outer - inner)), 2.0);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy * parallaxHeight;
    vec2 deltaTexCoords = P / numLayers;
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = 1.0 - texture(parallaxSampler, currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = 1.0 - texture(parallaxSampler, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = (1.0 - texture(parallaxSampler, prevTexCoords).r) - currentLayerDepth + layerDepth;
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

// Compute bumped normal from height map
vec3 computeBumpedNormal(vec2 uv) {
    float height = texture(bumpSampler, uv).r;
    float texelSize = 1.0 / 512.0;
    float heightU = texture(bumpSampler, uv + vec2(texelSize, 0.0)).r;
    float heightV = texture(bumpSampler, uv + vec2(0.0, texelSize)).r;
    float dFdu = (heightU - height) * bumpStrength;
    float dFdv = (heightV - height) * bumpStrength;

    vec3 tangent = TBN[0];
    vec3 bitangent = TBN[1];
    vec3 normal = TBN[2];

    vec3 perturbedNormal = normalize(
        tangent * (-dFdu) +
        bitangent * (-dFdv) +
        normal
    );

    return perturbedNormal;
}

void main() {
    // Transform view direction to tangent space for parallax mapping
    // vec3 tangentViewPos = transpose(TBN) * camPos;
    // vec3 tangentFragPos = transpose(TBN) * worldPos;
    // vec3 tangentViewDir = normalize(tangentViewPos - tangentFragPos);

    vec3 tangentViewDir = normalize(transpose(TBN) * (camPos - worldPos));

    // Apply texture repeat
    vec2 finalTexCoords = texCoords * vec2(repeatU, repeatV);

    // Apply parallax mapping if enabled
    if (useParallaxMap == 1) {
        vec2 baseTexCoords = fract(texCoords) * vec2(repeatU, repeatV);
        vec2 parallaxCoords = ParallaxMapping(baseTexCoords, tangentViewDir);

        vec2 normalizedCoords = parallaxCoords / vec2(repeatU, repeatV);
        if(normalizedCoords.x > 1.0 || normalizedCoords.y > 1.0 ||
           normalizedCoords.x < 0.0 || normalizedCoords.y < 0.0) {
            discard;
        }

        finalTexCoords = parallaxCoords;
    }

    // Wrap texture coordinates
    finalTexCoords = fract(finalTexCoords);

    // Determine final normal
    vec3 normal;
    if (useNormalMap == 1) {
        // Normal mapping
        vec3 tangentNormal = texture(normalSampler, finalTexCoords).rgb;
        tangentNormal = normalize(tangentNormal * 2.0 - 1.0);
        normal = normalize(TBN * tangentNormal);
    }
    else if (useBumpMap == 1) {
        // Bump mapping
        normal = computeBumpedNormal(finalTexCoords);
    }
    else {
        // Standard normal
        normal = normalize(worldNormal);
    }

    // Get base diffuse color
    vec3 objDiffuse = vec3(cDiffuse);
    if (useTexture == 1) {
        vec3 textureColor = texture(textureSampler, finalTexCoords).rgb;
        objDiffuse = mix(objDiffuse, textureColor, blend);
    }

    // Material colors
    vec3 objAmbient = vec3(cAmbient);
    vec3 objSpecular = vec3(cSpecular);

    // Start with ambient lighting
    vec3 tempColor = ambient * objAmbient;

    // Process each light
    for(int i = 0; i < numLights; i++) {
        int lightType = lights[i].type;
        vec3 intensity = lights[i].col;
        vec3 omega_l = vec3(0.0);
        vec3 attVec = lights[i].att;
        float d = distance(lights[i].pos, worldPos);
        float att = min(1.0, 1.0 / (attVec.x + d * attVec.y + d * d * attVec.z));

        // Determine light direction based on type
        switch(lightType) {
        case 0:  // Point light
            omega_l = normalize(lights[i].pos - worldPos);
            break;
        case 1:  // Directional light
            omega_l = normalize(-lights[i].dir);
            att = 1.0;
            break;
        case 2:  // Spot light
            omega_l = normalize(lights[i].pos - worldPos);
            float angle = acos(clamp(dot(normalize(lights[i].dir), -omega_l), -1.0, 1.0));
            float innerAngle = lights[i].angle - lights[i].penumbra;

            if (innerAngle <= angle && angle <= lights[i].angle) {
                intensity *= (1.0 - spotLightFalloff(angle, innerAngle, lights[i].angle));
            } else if (angle > lights[i].angle) {
                intensity *= 0.0;
            }
            break;
        }

        // Calculate reflection vector for specular
        vec3 r_l = normalize(reflect(-omega_l, normal));
        vec3 v = normalize(camPos - worldPos);

        // Diffuse component
        vec3 diffIntense = diffuse * objDiffuse * clamp(dot(normal, omega_l), 0.0, 1.0);

        // Specular component
        float clampedDot = clamp(dot(r_l, v), 0.0, 1.0);
        float powed = 1.0;
        if (!(clampedDot == 0.0 && shininess == 0.0)) {
            powed = pow(clampedDot, shininess);
        }
        vec3 specIntense = specular * objSpecular * powed;

        // Add light contribution
        tempColor += att * intensity * (diffIntense + specIntense);
    }

    // Clamp and output final color
    tempColor = clamp(tempColor, 0.0, 1.0);
    fragColor = vec4(tempColor, 1.0);
}
