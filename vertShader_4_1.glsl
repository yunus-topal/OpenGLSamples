#version 430

 layout (location = 0) in vec3 position;
 uniform float offset;

 uniform mat4 mv_matrix;
 uniform mat4 p_matrix;

 void main(void)
 { 
    vec4 initialPos = vec4(position, 1.0);
	initialPos.x += offset; // Apply the offset to the x-coordinate
	gl_Position = p_matrix * mv_matrix * initialPos;
 }
