#version 430

out vec4 fragColor;

uniform vec3  uColor;       // linear RGB
uniform float uIntensity;   // 1.0 by default; increase for a brighter “bulb”

void main()
{
    fragColor = vec4(uColor * uIntensity, 1.0);
}