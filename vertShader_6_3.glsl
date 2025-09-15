#version 430

layout(location=0) in vec3 position;
layout(location=2) in vec3 normal;

uniform mat4 mv_matrix;  // model * view
uniform mat4 p_matrix;   // projection
uniform mat3 n_matrix;   // normal matrix = transpose(inverse(mat3(mv)))

out vec3 vPosVS;   // position in view space
out vec3 vNormVS;  // normal in view space

void main() {
    vec4 posVS = mv_matrix * vec4(position, 1.0);
    vPosVS = posVS.xyz;
    vNormVS = normalize(n_matrix * normal);
    gl_Position = p_matrix * posVS;
}
