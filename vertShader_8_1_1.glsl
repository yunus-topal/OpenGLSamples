#version 430

layout(location = 0) in vec3 aPosition;   // matches your VAO's position attribute

uniform mat4 uMVP;                        // precomputed P * V * M

void main()
{
    gl_Position = uMVP * vec4(aPosition, 1.0);
}
