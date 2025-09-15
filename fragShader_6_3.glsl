#version 430

in vec3 vPosVS;
in vec3 vNormVS;

uniform vec3 lightPosVS;   // light position in view space
uniform vec3 ka = vec3(0.1);   // ambient
uniform vec3 kd = vec3(0.8);   // diffuse
uniform vec3 ks = vec3(0.2);   // spec
uniform float shininess = 64.0;

out vec4 fragColor;

void main() {
    vec3 N = normalize(vNormVS);
    vec3 L = normalize(lightPosVS - vPosVS);
    float NdotL = max(dot(N, L), 0.0);

    vec3 V = normalize(-vPosVS);              // view dir in view space
    vec3 R = reflect(-L, N);
    float spec = (NdotL > 0.0) ? pow(max(dot(R, V), 0.0), shininess) : 0.0;

    vec3 color = ka + kd * NdotL + ks * spec;
    fragColor = vec4(color, 1.0);
}
