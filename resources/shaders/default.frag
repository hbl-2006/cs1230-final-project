#version 330 core

in vec3 worldPos;
in vec3 worldNormal;
in vec2 texCoords;
in mat3 TBN;

out vec4 fragColor;
uniform float ambient;
uniform vec3 objAmbient;

uniform float diffuse;
uniform vec3 objDiffuse;

uniform float shininess;
uniform float specular;
uniform vec3 objSpecular;

uniform vec3 camPos;

struct Light {
    int type;
    vec3 pos;
    vec3 col;
    vec3 dir;
    vec3 att;
    float angle;
    float penumbra;
};

uniform Light lights[8];
uniform int numLights;

uniform bool useTexture;
uniform sampler2D textureSampler;
uniform float blend;
uniform float repeatU;
uniform float repeatV;

uniform bool useNormalMap;
uniform sampler2D normalSampler;

uniform bool useBumpMap;
uniform sampler2D bumpSampler;
uniform float bumpStrength;

uniform bool useParallaxMap;
uniform sampler2D parallaxSampler;
uniform float parallaxHeight;

float spotLightFalloff(float x, float inner, float outer)
{
    return -2 * pow(((x - inner) / (outer - inner)), 3)
         + 3 * pow(((x - inner) / (outer - inner)), 2);
}

vec2 ParallaxMapping(vec2 uv, vec3 viewDir)
{
    float h = texture(parallaxSampler, uv).r;
    vec2 p = viewDir.xy / viewDir.z * (h * parallaxHeight);
    return uv - p;
}

vec3 computeBumpedNormal(vec2 uv)
{
    float texel = 1.0 / 512.0;
    float h = texture(bumpSampler, uv).r;
    float hU = texture(bumpSampler, uv + vec2(texel, 0)).r;
    float hV = texture(bumpSampler, uv + vec2(0, texel)).r;

    float dU = (hU - h) * bumpStrength;
    float dV = (hV - h) * bumpStrength;

    vec3 T = TBN[0];
    vec3 B = TBN[1];
    vec3 N = TBN[2];

    return normalize(T * (-dU) + B * (-dV) + N);
}

void main()
{
    vec3 normedNormal = normalize(worldNormal);
    vec3 tangentCamPos = transpose(TBN) * camPos;
    vec3 tangentFragPos = transpose(TBN) * worldPos;
    vec3 tangentViewDir = normalize(tangentCamPos - tangentFragPos);

    vec2 uv = texCoords * vec2(repeatU, repeatV);

    if (useParallaxMap) {
        uv = ParallaxMapping(uv, tangentViewDir);
        if (uv.x < 0 || uv.y < 0 || uv.x > repeatU || uv.y > repeatV)
            discard;
    }

    uv = fract(uv);

    vec3 finalNormal;
    if (useNormalMap) {
        vec3 tn = texture(normalSampler, uv).rgb;
        tn = normalize(tn * 2.0 - 1.0);
        finalNormal = normalize(TBN * tn);
    }
    else if (useBumpMap) {
        finalNormal = computeBumpedNormal(uv);
    }
    else {
        finalNormal = normedNormal;
    }

    vec3 diffBase = objDiffuse;
    if (useTexture) {
        vec3 texC = texture(textureSampler, uv).rgb;
        diffBase = mix(diffBase, texC, blend);
    }

    vec3 tempColor = ambient * objAmbient;

    for (int i = 0; i < numLights; i++)
    {
        int lightType = lights[i].type;
        vec3 intensity = lights[i].col;
        vec3 omega_l = vec3(0.0);

        vec3 attVec = lights[i].att;
        float d = distance(lights[i].pos, worldPos);
        float att = min(1, 1/(attVec.x + d * attVec.y + d*d*attVec.z));

        switch (lightType)
        {
            case 0:
                omega_l = normalize(lights[i].pos - worldPos);
                break;

            case 1:
                omega_l = normalize(-lights[i].dir);
                att = 1.0;
                break;

            case 2:
                omega_l = normalize(lights[i].pos - worldPos);
                float angle = acos(clamp(dot(normalize(lights[i].dir), -omega_l), -1, 1));
                float inner = lights[i].angle - lights[i].penumbra;

                if (inner <= angle && angle <= lights[i].angle)
                    intensity *= (1 - spotLightFalloff(angle, inner, lights[i].angle));
                else if (angle > lights[i].angle)
                    intensity *= 0;
                break;
        }
        float NdotL = clamp(dot(finalNormal, omega_l), 0.0, 1.0);
        vec3 diff = diffuse * diffBase * NdotL;

        vec3 r_l = normalize(reflect(-omega_l, finalNormal));
        vec3 v = normalize(camPos - worldPos);

        float clamped = clamp(dot(r_l, v), 0, 1);
        float p = (clamped == 0 && shininess == 0) ? 1 : pow(clamped, shininess);

        vec3 specC = specular * objSpecular * p;

        tempColor += att * intensity * (diff + specC);
    }

    tempColor = clamp(tempColor, 0.0, 1.0);
    fragColor = vec4(tempColor, 1);
}
