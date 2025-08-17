#version 430

 layout (location = 0) in vec3 position;

 uniform mat4 mv_matrix;
 uniform mat4 p_matrix;

  out vec4 varyingColor;

  mat4 buildTranslate(float x, float y, float z);
  mat4 buildRotateX(float rad); 
  mat4 buildRotateY(float rad); 
  mat4 buildRotateZ(float rad);

 void main(void)
 { 
    vec4 initialPos = vec4(position, 1.0);
	gl_Position = p_matrix * mv_matrix * initialPos;

	varyingColor = initialPos * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
 }