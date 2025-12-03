#version 330 core

in vec2 uv;
in float lifeFraction;

out vec4 fragColor;

void main() {
    float dist = length(uv - vec2(0.5));
    float alpha = max(0.0, 1.0 - dist / 0.5) * max(0.0, 1.0 - dist / 0.5);

    vec3 color;

    if (lifeFraction > 0.9) {
        float t = (lifeFraction - 0.9) / 0.1;
        color = mix(vec3(1.0, 0.5, 0.0), vec3(1.0, 1.0, 0.75), t);
    } else if (lifeFraction > 0.85) {
        float t = (lifeFraction - 0.85) / 0.05;
        color = mix(vec3(1.0, 0.25, 0.0), vec3(1.0, 0.5, 0.0), t);
    } else if (lifeFraction > 0.5) {
        float t = (lifeFraction - 0.5) / 0.35;
        color = mix(vec3(0.5, 0.0, 0.0), vec3(1.0, 0.25, 0.0), t);
    } else {
        float t = lifeFraction / 0.5;
        color = mix(vec3(0.0, 0.0, 0.0), vec3(0.5, 0.0, 0.0), t);
    }

    fragColor = vec4(color, alpha);
}
