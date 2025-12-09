#version 330 core

// Task 5: declare "in" variables for the world-space position and normal,
//         received post-interpolation from the vertex shader
in vec3 worldPos;
in vec3 worldNormal;

// Task 10: declare an out vec4 for your output color
out vec4 fragColor;

// Task 12: declare relevant uniform(s) here, for ambient lighting
uniform float ambient;
uniform vec3 objAmbient;

// Task 13: declare relevant uniform(s) here, for diffuse lighting
uniform float diffuse;
uniform vec3 objDiffuse;
struct Light {
   // 0 is point, 1 is dir, 2 is spot
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

// Task 14: declare relevant uniform(s) here, for specular lighting
uniform float shininess;
uniform float specular;
uniform vec3 objSpecular;
uniform vec3 camPos;


float spotLightFalloff(float x, float inner, float outer)
{
    return -2 * pow(((x - inner) / (outer - inner)), 3)
           + 3 * pow(((x - inner) / (outer - inner)), 2);
}

void main() {
    // Remember that you need to renormalize vectors here if you want them to be normalized
    vec3 normedNormal = normalize(worldNormal);
    // ambient
    vec3 tempColor = vec3(0);
    tempColor = ambient * objAmbient;

    for(int i = 0; i < numLights; i++) {

       int lightType = lights[i].type;
       vec3 intensity = lights[i].col;
       vec3 omega_l = vec3(0.0);
       vec3 attVec = lights[i].att;
       float d = distance(lights[i].pos, worldPos);
       float att = min(1, 1/(attVec.x + d * attVec.y + d*d*attVec.z));
       switch(lightType) {
       case 0:
              omega_l = normalize(lights[i].pos - worldPos);
              break;
       case 1:
              omega_l = normalize(-lights[i].dir);
              att = 1.0;
              break;
       case 2:
              omega_l = normalize(lights[i].pos - worldPos);
              float angle = acos(clamp(dot(normalize(lights[i].dir), -omega_l),-1,1));
              float innerAngle = lights[i].angle - lights[i].penumbra;
              if (innerAngle <= angle && angle <= lights[i].angle) {
                  intensity *= (1 - spotLightFalloff(angle, innerAngle, lights[i].angle));
              } else if (angle > lights[i].angle) {
                     intensity *= 0;
              }
              break;
       }

       vec3 r_l = normalize(reflect(-omega_l, normedNormal));
       vec3 v = normalize(camPos - worldPos);
       //diffuse
       vec3 diffIntense = diffuse * objDiffuse * clamp(dot(normedNormal, omega_l), 0.0, 1.0);
       //specular
       float clampedDot = clamp(dot(r_l, v),0,1);
       float powed = 1;
       if (!(clampedDot == 0 && shininess == 0)) {
              powed = pow(clampedDot, shininess);
       }
       vec3 specIntense = specular * objSpecular * powed;
       tempColor += att * intensity * (diffIntense + specIntense);
    }
    tempColor = clamp(tempColor, 0.0, 1.0);
    fragColor = vec4(tempColor, 1);
}

